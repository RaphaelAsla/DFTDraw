#include "input.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "raylib.h"

#define to static_cast

namespace input {

    std::vector<std::complex<double>> readComplexNumbers(const std::string& filename) {
        std::ifstream                     file(filename);
        std::vector<std::complex<double>> complexNumbers;

        if (!file.is_open()) {
            std::cerr << "Could not open the file: " << filename << std::endl;
            return complexNumbers;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Remove any extra spaces from the line
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

            std::stringstream ss(line);
            double            real, imag;
            char              comma;

            if (ss >> real >> comma >> imag && comma == ',') {
                complexNumbers.emplace_back(real, imag);
            } else {
                std::cerr << "Failed to read values from line: " << line << std::endl;
            }
        }

        return complexNumbers;
    }

    std::vector<std::complex<double>> getUserPath() {
        const int screenWidth  = 1000;
        const int screenHeight = 1000;

        InitWindow(screenWidth, screenHeight, "Draw your path");

        std::vector<std::complex<double>> path;
        bool                              isDrawing = false;

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
                path.emplace_back(mousePos.x - screenWidth / 2.0f, mousePos.y - screenHeight / 2.0f);
            }

            if (!path.empty()) {
                for (size_t i = 1; i < path.size(); i++) {
                    Vector2 start = {to<float>(path[i - 1].real() + screenWidth / 2.0f), to<float>(path[i - 1].imag() + screenHeight / 2.0f)};
                    Vector2 end   = {to<float>(path[i].real() + screenWidth / 2.0f), to<float>(path[i].imag() + screenHeight / 2.0f)};
                    DrawLineEx(start, end, 2.0, RED);
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
