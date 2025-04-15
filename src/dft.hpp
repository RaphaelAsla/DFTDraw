#pragma once

#include <complex>
#include <tuple>
#include <vector>

namespace dft {

    using DFTResult = std::vector<std::tuple<std::complex<double>, double, double, double>>;

    DFTResult compute(const std::vector<std::complex<double>>& samples);

}  // namespace dft
