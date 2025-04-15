#include "visualizer.hpp"

#include <algorithm>
#include <cmath>

#include "raylib.h"
#include "raymath.h"

#define to static_cast

namespace viz {

    DFTVisualizer::DFTVisualizer(int width, int height, const char* title) : m_width(width), m_height(height) {
        InitWindow(width, height, title);
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
            double  angle        = e.freq * m_time + e.theta;
            Vector2 next         = {to<float>(current.x + e.radius * cos(angle)), to<float>(current.y + e.radius * sin(angle))};
            double  circleRadius = e.radius * m_zoom;

            Vector2 drawCurrent = {to<float>((current.x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((current.y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};
            Vector2 drawNext    = {to<float>((next.x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((next.y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};

            DrawCircleLines(to<int>(drawCurrent.x), to<int>(drawCurrent.y), circleRadius, {225, 225, 225, 100});
            // Arrow
            DrawLine(to<int>(drawCurrent.x), to<int>(drawCurrent.y), to<int>(drawNext.x), to<int>(drawNext.y), {225, 255, 255, 200});
            // Arrow tip
            double dx          = drawCurrent.x - drawNext.x;
            double dy          = drawCurrent.y - drawNext.y;
            double ux          = dx / e.radius;
            double uy          = dy / e.radius;
            double perpX       = -uy;
            double perpY       = ux;
            double arrowLength = std::clamp(e.radius * 0.2, 0.1, 20.0);
            double arrowWidth  = arrowLength;
            double baseX       = drawNext.x + ux * arrowLength;
            double baseY       = drawNext.y + uy * arrowLength;
            double leftX       = baseX + perpX * (arrowWidth / 2);
            double leftY       = baseY + perpY * (arrowWidth / 2);
            double rightX      = baseX - perpX * (arrowWidth / 2);
            double rightY      = baseY - perpY * (arrowWidth / 2);
            DrawTriangle(Vector2{to<float>(drawNext.x), to<float>(drawNext.y)}, Vector2{to<float>(leftX), to<float>(leftY)}, Vector2{to<float>(rightX), to<float>(rightY)}, {225, 255, 255, 255});

            current = next;
        }

        m_path.push_back(current);
        if (m_path.size() > 10000) {
            m_path.pop_back();
        }

        for (size_t i = 1; i < m_path.size(); i++) {
            Vector2 start = {to<float>((m_path[i - 1].x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((m_path[i - 1].y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};
            Vector2 end   = {to<float>((m_path[i].x + centerOffset.x) * m_zoom + center.x * (1 - m_zoom)), to<float>((m_path[i].y + centerOffset.y) * m_zoom + center.y * (1 - m_zoom))};
            // float   length = Vector2Distance(m_path[i - 1], m_path[i]);
            // if (length <= 8.0) {  // There is prolly a better way to do this but for now this is okay
            DrawLineEx(start, end, 2.0, BLUE);
            //}
        }

        DrawText("Press SPACE to lock/unlock camera", 10, 10, 20, DARKGRAY);

        char zoomText[32];
        snprintf(zoomText, sizeof(zoomText), "Zoom: %.1fx", m_zoom);
        DrawText(zoomText, m_width - 120, 10, 20, WHITE);
        DrawText("Use the scrollwheel to zoom in/out", 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

}  // namespace viz
