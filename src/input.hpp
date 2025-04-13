#pragma once

#include <complex>
#include <string>
#include <vector>

namespace input {

    std::vector<std::complex<float>> readComplexNumbers(const std::string& filename);
    std::vector<std::complex<float>> getUserPath();

}  // namespace input
