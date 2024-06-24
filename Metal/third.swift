//
//  main.swift
//  Contest
//
//  Created by Матвей Кузнецов on 31.07.2023.
//

import Foundation
import Metal
import CoreFoundation

func main() {
    let device = MTLCreateSystemDefaultDevice()!
    let commandQueue = device.makeCommandQueue()!

    let library = device.makeDefaultLibrary()!
    let kernelFunction = library.makeFunction(name: "computeDerivative")!

    var pipelineState: MTLComputePipelineState!
    do {
        pipelineState = try device.makeComputePipelineState(function: kernelFunction)
    } catch {
        print("Failed to create pipeline state: \(error)")
        return
    }
    
    let startTime = CFAbsoluteTimeGetCurrent()

    let width = 10000
    let height = 10000
    let inputArray = generateInputArray(width: width, height: height)
    let inputBuffer = device.makeBuffer(bytes: inputArray, length: inputArray.count * MemoryLayout<Float>.stride, options: [])!
    let outputBuffer = device.makeBuffer(length: inputArray.count * MemoryLayout<Float>.stride, options: [])!
    let dimensions = [UInt32(width), UInt32(height)]
    let dimensionsBuffer = device.makeBuffer(bytes: dimensions, length: 2 * MemoryLayout<UInt32>.stride, options: [])!

    let commandBuffer = commandQueue.makeCommandBuffer()!
    let computeEncoder = commandBuffer.makeComputeCommandEncoder()!

    computeEncoder.setComputePipelineState(pipelineState)
    computeEncoder.setBuffer(inputBuffer, offset: 0, index: 0)
    computeEncoder.setBuffer(outputBuffer, offset: 0, index: 1)
    computeEncoder.setBuffer(dimensionsBuffer, offset: 0, index: 2)

    let threadgroupSize = MTLSize(width: pipelineState.threadExecutionWidth, height: 1, depth: 1)
    let threadgroupCount = MTLSize(width: (width + threadgroupSize.width - 1) / threadgroupSize.width, height: height, depth: 1)

    computeEncoder.dispatchThreadgroups(threadgroupCount, threadsPerThreadgroup: threadgroupSize)
    computeEncoder.endEncoding()

    commandBuffer.commit()
    commandBuffer.waitUntilCompleted()
    
    let endTime = CFAbsoluteTimeGetCurrent()
    print("Time taken: \(endTime - startTime) seconds")

//    let outputArray = outputBuffer.contents().assumingMemoryBound(to: Float.self)
//    for i in 0..<height {
//        for j in 0..<width {
//            print("Output[\(i)][\(j)] = \(outputArray[i * width + j])")
//        }
//    }
}

func generateInputArray(width: Int, height: Int) -> [Float] {
    var array = [Float](repeating: 0, count: width * height)
    for i in 0..<height {
        for j in 0..<width {
            let x = Float(j) / Float(width) * 2 * Float.pi
            let y = Float(i) / Float(height) * 2 * Float.pi
            array[i * width + j] = sin(x) * cos(y)
        }
    }
    return array
}

main()
