//
//  func.metal
//  Contest
//
//  Created by Матвей Кузнецов on 21.06.2024.
//

#include <metal_stdlib>
using namespace metal;

kernel void computeKernel(device char* message [[buffer(0)]],
                          uint tid [[thread_position_in_grid]]) {
    const constant char* messages[] = {
        "Thread 0", "Thread 1", "Thread 2", "Thread 3"
    };
    const constant char* msg = messages[tid];
    for (int i = 0; msg[i] != '\0'; ++i) {
        message[tid * 50 + i] = msg[i]; // Предполагаем, что каждое сообщение не длиннее 50 символов
    }
}
