//
//  func.metal
//  Contest
//
//  Created by Матвей Кузнецов on 21.06.2024.
//

#include <metal_stdlib>
using namespace metal;

kernel void computeSum(device const float* data [[buffer(0)]],
                       device float* result [[buffer(1)]],
                       uint tid [[thread_position_in_grid]]) {
    int arraySize = 10000000;
    float sum = 0.0;
    for (int i = tid; i < arraySize; i += arraySize) {
        sum += data[i];
    }
    atomic_fetch_add_explicit((device atomic_float*)result, sum, memory_order_relaxed);
}
