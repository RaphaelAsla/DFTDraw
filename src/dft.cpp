#include "dft.hpp"

#include <cmath>

namespace dft {

    constexpr float PI = 3.14159265358979323846f;

    DFTResult compute(const std::vector<std::complex<float>>& samples) {
        const size_t N = samples.size();
        DFTResult    X(N);

        for (size_t k = 0; k < N; k++) {
            std::complex<float> sum = 0;
            for (size_t n = 0; n < N; n++) {
                float angle = (2 * PI * k * n) / N;
                sum += samples[n] * (std::cos(angle) - std::complex<float>(0, 1) * std::sin(angle));
            }

            sum /= N;

            float amp   = std::sqrt(std::pow(sum.real(), 2) + std::pow(sum.imag(), 2));
            float phase = std::atan2(sum.imag(), sum.real());
            float freq  = k;

            X[k] = {sum, amp, phase, freq};
        }

        return X;
    }

}  // namespace dft
