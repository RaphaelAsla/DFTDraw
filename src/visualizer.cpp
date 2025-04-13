#include "visualizer.hpp"

#include <cmath>

#include "raylib.h"
#include "raymath.h"

namespace viz {

    DFTVisualizer::DFTVisualizer(int width, int height, const char* title) : m_width(width), m_height(height) {
        InitWindow(width, height, title);
        SetTargetFPS(60);
    }

    DFTVisualizer::~DFTVisualizer() {
        CloseWindow();
    }

    void DFTVisualizer::run(const std::vector<epicycloid::Epicycloid>& epicycloids, size_t sampleCount) {
        while (!WindowShouldClose()) {
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

    void DFTVisualizer::drawFrame(const std::vector<epicycloid::Epicycloid>& epicycloids) {
        BeginDrawing();
        ClearBackground(BLACK);

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            m_zoom *= (1.0f + wheel * 0.1f);
            m_zoom = std::max(0.1f, std::min(m_zoom, 10.0f));
        }

        Vector2 center  = {m_width / 2.0f, m_height / 2.0f};
        Vector2 current = center;

        Vector2 finalPosition = center;

        if (m_lock) {
            for (const auto& e : epicycloids) {
                float angle = e.freq * m_time + e.theta;
                finalPosition.x += e.radius * cos(angle);
                finalPosition.y += e.radius * sin(angle);
            }
        }

        Vector2 centerOffset = (m_lock) ? center - finalPosition : Vector2{0.0f};

        for (const auto& e : epicycloids) {
            float   angle        = e.freq * m_time + e.theta;
            Vector2 next         = {current.x + e.radius * cos(angle), current.y + e.radius * sin(angle)};
            float   circleRadius = e.radius * m_zoom;

            Vector2 drawCurrent = {(current.x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom), (current.y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom)};
            Vector2 drawNext    = {(next.x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom), (next.y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom)};

            DrawCircleLines(static_cast<int>(drawCurrent.x), static_cast<int>(drawCurrent.y), circleRadius, LIGHTGRAY);
            DrawLine(static_cast<int>(drawCurrent.x), static_cast<int>(drawCurrent.y), static_cast<int>(drawNext.x), static_cast<int>(drawNext.y), GRAY);

            current = next;
        }

        m_path.insert(m_path.begin(), current);
        if (m_path.size() > 10000) {
            m_path.pop_back();
        }

        for (size_t i = 1; i < m_path.size(); i++) {
            Vector2 start = {(m_path[i - 1].x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom), (m_path[i - 1].y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom)};
            Vector2 end   = {(m_path[i].x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom), (m_path[i].y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom)};
            DrawLineV(start, end, RED);
        }

        DrawText("Press SPACE to lock/unlock camera", 10, 10, 20, DARKGRAY);

        char zoomText[32];
        snprintf(zoomText, sizeof(zoomText), "Zoom: %.1fx", m_zoom);
        DrawText(zoomText, m_width - 120, 10, 20, WHITE);
        DrawText("Use the scrollwheel to zoom in/out", 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

}  // namespace viz
