#include "dft.hpp"
#include "epicycloid.hpp"
#include "input.hpp"
#include "visualizer.hpp"

int main() {
    const int screenWidth  = 1000;
    const int screenHeight = 1000;

    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    /* Yes, I know, I can just creaete a single window for both input and dft but oh well, can't bother fixing it now.
       Yes, I also know exiting keybinds are messed up because of that but oh well, maybe another time gtg to sleep now. */
    while (true) {
        auto samples = input::getUserPath();

        /* If you use this make sure to un-comment lines 102 - 104 in visualizer.cpp.
           I know it's not convinient but I wanted to make this work because it just looks cool, can't bother with this anymore */
        // auto samples = input::readComplexNumbers("inp.txt");

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
