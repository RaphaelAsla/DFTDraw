#include "dft_draw.hpp"
#include <iostream>

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "DFT Draw");

    while (!WindowShouldClose()) {
        auto samples = getUserPathInExistingWindow();

        if (samples.empty()) {
            continue;
        }

        auto dftResult = computeDFT(samples);
        auto epicycloids = createEpicycloids(dftResult);

        runVisualizationInExistingWindow(epicycloids, samples.size());
    }

    CloseWindow();
    return 0;
}
