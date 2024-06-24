//
//  func.metal
//  Contest
//
//  Created by Матвей Кузнецов on 21.06.2024.
//

#include <metal_stdlib>
using namespace metal;

kernel void computeDerivative(const device float* input [[buffer(0)]],
                              device float* output [[buffer(1)]],
                              constant uint2& dimensions [[buffer(2)]],
                              uint2 id [[thread_position_in_grid]]) {
    uint width = dimensions.x;
    uint height = dimensions.y;

    if (id.x > 0 && id.x < width - 1 && id.y < height) {
        uint index = id.y * width + id.x;
        output[index] = (input[index + 1] - input[index - 1]) / 2.0f;
    }
}
