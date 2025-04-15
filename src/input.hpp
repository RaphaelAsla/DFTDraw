#pragma once

#include <complex>
#include <string>
#include <vector>

namespace input {

    std::vector<std::complex<double>> readComplexNumbers(const std::string& filename);
    std::vector<std::complex<double>> getUserPath();

}  // namespace input
