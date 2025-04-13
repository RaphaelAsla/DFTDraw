#pragma once

#include <complex>
#include <tuple>
#include <vector>

namespace dft {

    using DFTResult = std::vector<std::tuple<std::complex<float>, float, float, float>>;

    DFTResult compute(const std::vector<std::complex<float>>& samples);

}  // namespace dft
