#include "DiskThickness.h"

#include <cmath>
#include <vector>

namespace{
    constexpr float baseHalfDepth = 0.5f;
    constexpr float peakHalfDepth = 10.0f * baseHalfDepth;
    constexpr float scaleLength = 5.0f;
}

float DiskThickness::halfDepth(int ring){
    static std::vector<float> cache;

    if(static_cast<std::size_t>(ring) >= cache.size()){
        cache.resize(ring + 1, -1.0f);
    }

    float& cached = cache[ring];

    if(cached < 0.0f){
        cached =
            baseHalfDepth +
            (peakHalfDepth - baseHalfDepth) *
            std::exp(-static_cast<float>(ring) / scaleLength);
    }

    return cached;
}
