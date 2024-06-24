//
//  func.metal
//  Contest
//
//  Created by Матвей Кузнецов on 21.06.2024.
//

#include <metal_stdlib>
using namespace metal;

kernel void matrixMultiplication(const device float* A [[buffer(0)]],
                                  const device float* B [[buffer(1)]],
                                  device float* C [[buffer(2)]],
                                  constant uint2& dimensions [[buffer(3)]],
                                  uint2 id [[thread_position_in_grid]]) {
    uint widthA = dimensions.x;
    uint heightA = dimensions.y;
    uint widthB = dimensions.x;
    uint heightB = dimensions.y;

    if (id.x < widthB && id.y < heightA) {
        float sum = 0.0;
        for (uint k = 0; k < widthA; ++k) {
            sum += A[id.y * widthA + k] * B[k * widthB + id.x];
        }
        C[id.y * widthB + id.x] = sum;
    }
}
