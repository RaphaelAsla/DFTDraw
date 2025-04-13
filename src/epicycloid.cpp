#include "epicycloid.hpp"

#include <algorithm>
#include <cmath>

namespace epicycloid {

    Epicycloid::Epicycloid(float amp, float phase, float freq) : radius(amp), theta(phase), freq(freq) {}

    std::vector<Epicycloid> createFromDFT(const dft::DFTResult& dftResult) {
        std::vector<Epicycloid> epicycloids;
        epicycloids.reserve(dftResult.size());

        for (const auto& [complex, amp, phase, freq] : dftResult) {
            epicycloids.emplace_back(amp, phase, freq);
        }

        std::sort(epicycloids.begin(), epicycloids.end(), [](const Epicycloid& a, const Epicycloid& b) { return a.radius > b.radius; });

        return epicycloids;
    }

}  // namespace epicycloid
