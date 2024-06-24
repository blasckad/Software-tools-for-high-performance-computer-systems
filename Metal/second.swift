//
//  main.swift
//  Contest
//
//  Created by Матвей Кузнецов on 31.07.2023.
//

import Foundation
import Metal
import CoreFoundation

// Инициализация Metal
guard let device = MTLCreateSystemDefaultDevice() else {
    print("Metal is not supported on this device")
    exit(1)
}
let commandQueue = device.makeCommandQueue()!

// Загрузка шейдера
guard let library = device.makeDefaultLibrary() else {
    print("Failed to load default library")
    exit(1)
}
guard let kernelFunction = library.makeFunction(name: "computeSum") else {
    print("Failed to load kernel function")
    exit(1)
}

// Создание вычислительного пайплайна
var computePipelineState: MTLComputePipelineState!
do {
    computePipelineState = try device.makeComputePipelineState(function: kernelFunction)
} catch {
    print("Failed to create compute pipeline state: \(error)")
    exit(1)
}

let startTime = CFAbsoluteTimeGetCurrent()

// Создание массива данных
let arraySize = 10000000
var array = (0..<arraySize).map { _ in Float(arc4random_uniform(100)) }

// Создание буферов для данных и результата
let dataBuffer = device.makeBuffer(bytes: array, length: arraySize * MemoryLayout<Float>.stride, options: [])!
let resultBuffer = device.makeBuffer(length: MemoryLayout<Float>.stride, options: [])!

// Выполнение вычислений
let commandBuffer = commandQueue.makeCommandBuffer()!
let computeEncoder = commandBuffer.makeComputeCommandEncoder()!

computeEncoder.setComputePipelineState(computePipelineState)
computeEncoder.setBuffer(dataBuffer, offset: 0, index: 0)
computeEncoder.setBuffer(resultBuffer, offset: 0, index: 1)

let threadgroupSize = MTLSize(width: 128, height: 1, depth: 1)
let threadgroupCount = MTLSize(width: (arraySize + threadgroupSize.width - 1) / threadgroupSize.width, height: 1, depth: 1)

computeEncoder.dispatchThreadgroups(threadgroupCount, threadsPerThreadgroup: threadgroupSize)
computeEncoder.endEncoding()

commandBuffer.addCompletedHandler { _ in
    let resultPointer = resultBuffer.contents().assumingMemoryBound(to: Float.self)
    let sum = resultPointer.pointee
    print("Sum: \(sum)")
}

commandBuffer.commit()
commandBuffer.waitUntilCompleted()

let endTime = CFAbsoluteTimeGetCurrent()
print("Time taken: \(endTime - startTime) seconds")
