#include "dft.hpp"

#include <cmath>

#include "raymath.h"

namespace dft {

    DFTResult compute(const std::vector<std::complex<double>>& samples) {
        const size_t N = samples.size();
        DFTResult    X(N);

        for (size_t k = 0; k < N; k++) {
            std::complex<double> sum = 0;
            for (size_t n = 0; n < N; n++) {
                double angle = (2 * PI * k * n) / N;
                sum += samples[n] * (std::cos(angle) - std::complex<double>(0, 1) * std::sin(angle));
            }

            sum /= N;

            double amp   = std::sqrt(std::pow(sum.real(), 2) + std::pow(sum.imag(), 2));
            double phase = std::atan2(sum.imag(), sum.real());
            double freq  = k;

            X[k] = {sum, amp, phase, freq};
        }

        return X;
    }

}  // namespace dft
