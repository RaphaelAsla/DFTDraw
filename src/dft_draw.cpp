#include "dft_draw.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "raymath.h"

#define to static_cast

Epicycloid::Epicycloid(double amp, double phase, double freq) : radius(amp), theta(phase), freq(freq) {}

DFTResult computeDFT(const std::vector<Complex>& samples) {
    const size_t N = samples.size();
    DFTResult X(N);

    for (size_t k = 0; k < N; k++) {
        Complex sum = 0;
        for (size_t n = 0; n < N; n++) {
            double angle = (2 * PI * k * n) / N;
            sum += samples[n] * (std::cos(angle) - Complex(0, 1) * std::sin(angle));
        }

        sum /= N;

        double amp   = std::sqrt(std::pow(sum.real(), 2) + std::pow(sum.imag(), 2));
        double phase = std::atan2(sum.imag(), sum.real());
        double freq  = k;

        X[k] = {sum, amp, phase, freq};
    }

    return X;
}

std::vector<Epicycloid> createEpicycloids(const DFTResult& dftResult) {
    std::vector<Epicycloid> epicycloids;
    epicycloids.reserve(dftResult.size());

    for (const auto& [complex, amp, phase, freq] : dftResult) {
        epicycloids.emplace_back(amp, phase, freq);
    }

    std::sort(epicycloids.begin(), epicycloids.end(), [](const Epicycloid& a, const Epicycloid& b) {
        return a.radius > b.radius;
    });

    return epicycloids;
}

std::vector<Complex> readComplexNumbers(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Complex> complexNumbers;

    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return complexNumbers;
    }

    std::string line;
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

        std::stringstream ss(line);
        double real, imag;
        char comma;

        if (ss >> real >> comma >> imag && comma == ',') {
            complexNumbers.emplace_back(real, imag);
        } else {
            std::cerr << "Failed to read values from line: " << line << std::endl;
        }
    }

    return complexNumbers;
}

std::vector<Complex> getUserPath() {
    const int screenWidth  = 1000;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Draw your path");

    std::vector<Complex> path;
    bool isDrawing = false;

    while (!WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) {
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
        DrawText("Press ESC to exit", 10, 70, 20, DARKGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER) && !path.empty()) {
            break;
        }
    }

    CloseWindow();
    return path;
}

DFTVisualizer::DFTVisualizer(int width, int height, const char* title) : m_width(width), m_height(height) {
    InitWindow(width, height, title);
}

DFTVisualizer::~DFTVisualizer() {
    CloseWindow();
}

void DFTVisualizer::run(const std::vector<Epicycloid>& epicycloids, size_t sampleCount) {
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) {
        if (IsKeyPressed(KEY_SPACE)) {
            m_lock = !m_lock;
            m_zoom = m_lock ? m_zoom : 1.0f;
        }

        m_time += (2 * PI) / sampleCount;

        drawFrame(epicycloids);

        if (m_time > 2 * PI) {
            m_time = 0.0;
        }
    }
}

void DFTVisualizer::drawFrame(const std::vector<Epicycloid>& epicycloids) {
    BeginDrawing();
    ClearBackground(BLACK);

    double wheel = GetMouseWheelMove();
    if (wheel != 0) {
        m_zoom *= (1.0 + wheel * 0.1);
        m_zoom = std::max(0.1, std::min(m_zoom, 500.0));
    }

    Vector2 center  = {m_width / 2.0f, m_height / 2.0f};
    Vector2 current = center;

    Vector2 finalPosition = center;

    if (m_lock) {
        for (const auto& e : epicycloids) {
            double angle = e.freq * m_time + e.theta;
            finalPosition.x += e.radius * cos(angle);
            finalPosition.y += e.radius * sin(angle);
        }
    }

    Vector2 centerOffset = (m_lock) ? center - finalPosition : Vector2{0.0f};

    for (const auto& e : epicycloids) {
        double angle        = e.freq * m_time + e.theta;
        Vector2 next        = {to<float>(current.x + e.radius * cos(angle)), to<float>(current.y + e.radius * sin(angle))};
        double circleRadius = e.radius * m_zoom;

        Vector2 drawCurrent = {to<float>((current.x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((current.y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};
        Vector2 drawNext    = {to<float>((next.x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((next.y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};

        DrawCircleLines(drawCurrent.x, drawCurrent.y, circleRadius, {225, 225, 225, 100});
        // Arrow base
        DrawLine(drawCurrent.x, drawCurrent.y, drawNext.x, drawNext.y, {225, 255, 255, 200});
        // Arrow tip
        Vector2 direction     = drawCurrent - drawNext;
        Vector2 unitDirection = direction / e.radius;  // Normalize
        Vector2 perpendicular = {-unitDirection.y, unitDirection.x};
        double arrowLength    = std::clamp(e.radius * 0.2, 0.1, 20.0);
        double arrowWidth     = arrowLength;
        Vector2 arrowBase     = drawNext + unitDirection * arrowLength;
        Vector2 leftWing      = arrowBase + perpendicular * (arrowWidth / 2);
        Vector2 rightWing     = arrowBase - perpendicular * (arrowWidth / 2);

        DrawTriangle(drawNext, leftWing, rightWing, {225, 255, 255, 255});

        current = next;
    }

    m_path.push_back(current);
    if (m_path.size() > 10000) {
        m_path.pop_back();
    }

    for (size_t i = 1; i < m_path.size(); i++) {
        Vector2 start = {to<float>((m_path[i - 1].x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((m_path[i - 1].y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};
        Vector2 end   = {to<float>((m_path[i].x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((m_path[i].y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};
        // float length = Vector2Distance(m_path[i - 1], m_path[i]);
        // if (length <= 8.0) {  // There is prolly a better way to do this but for now this is okay
        // DrawLineEx(start, end, 2.0, BLUE);
        // }
        DrawLineEx(start, end, 2.0, BLUE);
    }

    DrawText("Press SPACE to lock/unlock camera", 10, 10, 20, DARKGRAY);
    DrawText("Press ESC to exit", 10, 40, 20, DARKGRAY);

    char zoomText[32];
    snprintf(zoomText, sizeof(zoomText), "Zoom: %.1fx", m_zoom);
    DrawText(zoomText, m_width - 120, 10, 20, WHITE);
    DrawText("Use the scrollwheel to zoom in/out", 10, 70, 20, DARKGRAY);

    EndDrawing();
}

std::vector<Complex> getUserPathInExistingWindow() {
    const int screenWidth  = 1000;
    const int screenHeight = 1000;

    std::vector<Complex> path;
    bool isDrawing  = false;
    bool shouldExit = false;

    while (!WindowShouldClose() && !shouldExit) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_ESCAPE)) {
            shouldExit = true;
            path.clear();
        }

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
        DrawText("Press ESC to exit", 10, 70, 20, DARKGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER) && !path.empty()) {
            break;
        }
    }

    // Must wait for the ENTER key to be released before entering the visualization or it will break immediately
    while (IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Release ENTER key...", 10, 10, 20, WHITE);
        EndDrawing();
    }

    return path;
}

void runVisualizationInExistingWindow(const std::vector<Epicycloid>& epicycloids, size_t sampleCount) {
    double time = 0.0f;
    double zoom = 1.0f;
    bool lock   = false;
    std::vector<Vector2> path;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Starting visualization...", 10, 10, 20, WHITE);
    EndDrawing();

    while (!WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) {
        if (IsKeyPressed(KEY_SPACE)) {
            lock = !lock;
            zoom = lock ? zoom : 1.0f;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            break;
        }

        time += (2 * PI) / sampleCount;

        BeginDrawing();
        ClearBackground(BLACK);

        double wheel = GetMouseWheelMove();
        if (wheel != 0) {
            zoom *= (1.0 + wheel * 0.1);
            zoom = std::max(0.1, std::min(zoom, 500.0));
        }

        Vector2 center        = {500.0f, 500.0f};
        Vector2 current       = center;
        Vector2 finalPosition = center;

        if (lock) {
            for (const auto& e : epicycloids) {
                double angle = e.freq * time + e.theta;
                finalPosition.x += e.radius * cos(angle);
                finalPosition.y += e.radius * sin(angle);
            }
        }

        Vector2 centerOffset = (lock) ? center - finalPosition : Vector2{0.0f};

        for (const auto& e : epicycloids) {
            if (e.radius < 0.5)
                continue;

            double angle        = e.freq * time + e.theta;
            Vector2 next        = {to<float>(current.x + e.radius * cos(angle)), to<float>(current.y + e.radius * sin(angle))};
            double circleRadius = e.radius * zoom;

            Vector2 drawCurrent = {to<float>((current.x + centerOffset.x) * zoom + center.x * (1 - zoom)), to<float>((current.y + centerOffset.y) * zoom + center.y * (1 - zoom))};
            Vector2 drawNext    = {to<float>((next.x + centerOffset.x) * zoom + center.x * (1 - zoom)), to<float>((next.y + centerOffset.y) * zoom + center.y * (1 - zoom))};

            if (circleRadius > 1.0) {
                DrawCircleLines(drawCurrent.x, drawCurrent.y, circleRadius, {225, 225, 225, 100});
            }
            DrawLine(drawCurrent.x, drawCurrent.y, drawNext.x, drawNext.y, {225, 255, 255, 200});

            if (e.radius > 2.0) {
                Vector2 direction = drawCurrent - drawNext;
                if (e.radius > 0.001) {
                    Vector2 unitDirection = direction / e.radius;
                    Vector2 perpendicular = {-unitDirection.y, unitDirection.x};
                    double arrowLength    = std::clamp(e.radius * 0.2, 0.1, 20.0);
                    double arrowWidth     = arrowLength;
                    Vector2 arrowBase     = drawNext + unitDirection * arrowLength;
                    Vector2 leftWing      = arrowBase + perpendicular * (arrowWidth / 2);
                    Vector2 rightWing     = arrowBase - perpendicular * (arrowWidth / 2);

                    DrawTriangle(drawNext, leftWing, rightWing, {225, 255, 255, 255});
                }
            }

            current = next;
        }

        path.push_back(current);
        if (path.size() > 10000) {
            path.erase(path.begin());
        }

        for (size_t i = 1; i < path.size(); i++) {
            Vector2 start = {to<float>((path[i - 1].x + centerOffset.x) * zoom + center.x * (1 - zoom)), to<float>((path[i - 1].y + centerOffset.y) * zoom + center.y * (1 - zoom))};
            Vector2 end   = {to<float>((path[i].x + centerOffset.x) * zoom + center.x * (1 - zoom)), to<float>((path[i].y + centerOffset.y) * zoom + center.y * (1 - zoom))};
            DrawLineEx(start, end, 2.0, BLUE);
        }

        DrawText("Press SPACE to lock/unlock camera", 10, 10, 20, DARKGRAY);
        DrawText("Press ENTER to draw new path", 10, 40, 20, DARKGRAY);
        DrawText("Press ESC to exit", 10, 70, 20, DARKGRAY);

        char zoomText[32];
        snprintf(zoomText, sizeof(zoomText), "Zoom: %.1fx", zoom);
        DrawText(zoomText, 1000 - 120, 10, 20, WHITE);
        DrawText("Use the scrollwheel to zoom in/out", 10, 100, 20, DARKGRAY);

        char epicycloidsText[64];
        snprintf(epicycloidsText, sizeof(epicycloidsText), "Epicycloids: %zu", epicycloids.size());
        DrawText(epicycloidsText, 10, 130, 20, DARKGRAY);

        EndDrawing();

        if (time > 2 * PI) {
            time = 0.0;
            path.clear();
        }
    }
}
