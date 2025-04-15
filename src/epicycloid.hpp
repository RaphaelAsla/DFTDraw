#pragma once

#include <vector>

#include "dft.hpp"

namespace epicycloid {

    struct Epicycloid {
        Epicycloid(double amp, double phase, double freq);

        double radius;
        double theta;
        double freq;
    };

    std::vector<Epicycloid> createFromDFT(const dft::DFTResult& dftResult);

}  // namespace epicycloid
