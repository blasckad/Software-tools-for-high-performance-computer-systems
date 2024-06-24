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
    let kernelFunction = library.makeFunction(name: "matrixMultiplication")!

    var pipelineState: MTLComputePipelineState!
    do {
        pipelineState = try device.makeComputePipelineState(function: kernelFunction)
    } catch {
        print("Failed to create pipeline state: \(error)")
        return
    }
    
    let startTime = CFAbsoluteTimeGetCurrent()

    let width = 1000
    let height = 1000
    let matrixA = generateMatrix(width: width, height: height)
    let matrixB = generateMatrix(width: width, height: height)
    let matrixC = [Float](repeating: 0, count: width * height)

    let bufferA = device.makeBuffer(bytes: matrixA, length: matrixA.count * MemoryLayout<Float>.stride, options: [])!
    let bufferB = device.makeBuffer(bytes: matrixB, length: matrixB.count * MemoryLayout<Float>.stride, options: [])!
    let bufferC = device.makeBuffer(bytes: matrixC, length: matrixC.count * MemoryLayout<Float>.stride, options: [])!
    let dimensions = [UInt32(width), UInt32(height)]
    let dimensionsBuffer = device.makeBuffer(bytes: dimensions, length: 2 * MemoryLayout<UInt32>.stride, options: [])!

    let commandBuffer = commandQueue.makeCommandBuffer()!
    let computeEncoder = commandBuffer.makeComputeCommandEncoder()!

    computeEncoder.setComputePipelineState(pipelineState)
    computeEncoder.setBuffer(bufferA, offset: 0, index: 0)
    computeEncoder.setBuffer(bufferB, offset: 0, index: 1)
    computeEncoder.setBuffer(bufferC, offset: 0, index: 2)
    computeEncoder.setBuffer(dimensionsBuffer, offset: 0, index: 3)

    let threadgroupSize = MTLSize(width: pipelineState.threadExecutionWidth, height: 1, depth: 1)
    let threadgroupCount = MTLSize(width: (width + threadgroupSize.width - 1) / threadgroupSize.width, height: height, depth: 1)

    computeEncoder.dispatchThreadgroups(threadgroupCount, threadsPerThreadgroup: threadgroupSize)
    computeEncoder.endEncoding()

    commandBuffer.commit()
    commandBuffer.waitUntilCompleted()

    let resultMatrix = bufferC.contents().assumingMemoryBound(to: Float.self)
//    for i in 0..<height {
//        for j in 0..<width {
//            print("C[\(i)][\(j)] = \(resultMatrix[i * width + j])")
//        }
//    }
    let endTime = CFAbsoluteTimeGetCurrent()
    print("Time taken: \(endTime - startTime) seconds")
}

func generateMatrix(width: Int, height: Int) -> [Float] {
    var matrix = [Float](repeating: 0, count: width * height)
    for i in 0..<height {
        for j in 0..<width {
            matrix[i * width + j] = Float.random(in: 0...1)
        }
    }
    return matrix
}

main()
