#include "input.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "raylib.h"

namespace input {

    std::vector<std::complex<float>> readComplexNumbers(const std::string& filename) {
        std::ifstream                    file(filename);
        std::vector<std::complex<float>> complexNumbers;

        if (!file.is_open()) {
            std::cerr << "Could not open the file: " << filename << std::endl;
            return complexNumbers;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Remove any extra spaces from the line
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

            std::stringstream ss(line);
            float             real, imag;
            char              comma;

            if (ss >> real >> comma >> imag && comma == ',') {
                complexNumbers.emplace_back(real, imag);
            } else {
                std::cerr << "Failed to read values from line: " << line << std::endl;
            }
        }

        return complexNumbers;
    }

    std::vector<std::complex<float>> getUserPath() {
        const int screenWidth  = 1500;
        const int screenHeight = 1000;

        InitWindow(screenWidth, screenHeight, "Draw your path");
        SetTargetFPS(60);

        std::vector<std::complex<float>> path;
        bool                             isDrawing = false;

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isDrawing = true;
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                isDrawing = false;
            }

            if (isDrawing) {
                Vector2 mousePos = GetMousePosition();
                float   x        = (mousePos.x - screenWidth / 2.0f) / (screenWidth / 2.0f);
                float   y        = (mousePos.y - screenHeight / 2.0f) / (screenHeight / 2.0f);
                path.emplace_back(x, y);
            }

            if (!path.empty()) {
                for (size_t i = 1; i < path.size(); i++) {
                    Vector2 start = {(path[i - 1].real() * screenWidth / 2.0f) + screenWidth / 2.0f, (path[i - 1].imag() * screenHeight / 2.0f) + screenHeight / 2.0f};
                    Vector2 end   = {(path[i].real() * screenWidth / 2.0f) + screenWidth / 2.0f, (path[i].imag() * screenHeight / 2.0f) + screenHeight / 2.0f};
                    DrawLineV(start, end, RED);
                }
            }

            DrawText("Draw your path with the mouse", 10, 10, 20, DARKGRAY);
            DrawText("Press ENTER when done", 10, 40, 20, DARKGRAY);

            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                break;
            }
        }

        CloseWindow();
        return path;
    }

}  // namespace input
