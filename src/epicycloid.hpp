#pragma once

#include <vector>

#include "dft.hpp"

namespace epicycloid {

    struct Epicycloid {
        Epicycloid(float amp, float phase, float freq);

        float radius;
        float theta;
        float freq;
    };

    std::vector<Epicycloid> createFromDFT(const dft::DFTResult& dftResult);

}  // namespace epicycloid
