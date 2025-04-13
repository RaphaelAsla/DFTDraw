#include "dft.hpp"
#include "epicycloid.hpp"
#include "input.hpp"
#include "visualizer.hpp"

int main() {
    const int screenWidth  = 1000;
    const int screenHeight = 1000;

    while (true) {
        auto samples = input::getUserPath();

        if (samples.empty()) {
            return 1;
        }

        auto dftResult = dft::compute(samples);

        auto epicycloids = epicycloid::createFromDFT(dftResult);

        viz::DFTVisualizer visualizer(screenWidth, screenHeight, "DFT Epicycles 2D");
        visualizer.run(epicycloids, samples.size());
    }

    return 0;
}
