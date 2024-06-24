//
//  main.swift
//  Contest
//
//  Created by Матвей Кузнецов on 31.07.2023.
//

import Foundation
import Metal

// Инициализация Metal
let device = MTLCreateSystemDefaultDevice()!
let commandQueue = device.makeCommandQueue()!

// Загрузка шейдера
let library = device.makeDefaultLibrary()!
let kernelFunction = library.makeFunction(name: "computeKernel")!

// Создание вычислительного пайплайна
var computePipelineState: MTLComputePipelineState!
do {
    computePipelineState = try device.makeComputePipelineState(function: kernelFunction)
} catch {
    print("Failed to create compute pipeline state: \(error)")
    exit(1)
}

// Создание буфера для хранения сообщений
let bufferSize = 4 * 50 // 4 потока, каждый поток может иметь до 50 символов
let buffer = device.makeBuffer(length: bufferSize, options: [])!

// Выполнение вычислений
let commandBuffer = commandQueue.makeCommandBuffer()!
let computeEncoder = commandBuffer.makeComputeCommandEncoder()!

computeEncoder.setComputePipelineState(computePipelineState)
computeEncoder.setBuffer(buffer, offset: 0, index: 0)

let threadgroupSize = MTLSizeMake(4, 1, 1)
let threadgroupCount = MTLSizeMake(1, 1, 1)

computeEncoder.dispatchThreadgroups(threadgroupCount, threadsPerThreadgroup: threadgroupSize)
computeEncoder.endEncoding()

commandBuffer.addCompletedHandler { _ in
    let bufferPointer = buffer.contents()
    for i in 0..<4 {
        let messageData = Data(bytes: bufferPointer + i * 50, count: 50)
        if let message = String(data: messageData, encoding: .utf8) {
            print(message)
        }
    }
}

commandBuffer.commit()
commandBuffer.waitUntilCompleted()
