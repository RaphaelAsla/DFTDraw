#pragma once

#include <raylib.h>

#include <vector>

#include "epicycloid.hpp"

namespace viz {

    class DFTVisualizer {
      public:
        DFTVisualizer(int width, int height, const char* title);
        ~DFTVisualizer();

        void run(const std::vector<epicycloid::Epicycloid>& epicycloids, size_t sampleCount);

      private:
        void drawFrame(const std::vector<epicycloid::Epicycloid>& epicycloids);

        int                  m_width;
        int                  m_height;
        float                m_time = 0.0f;
        float                m_zoom = 1.0f;
        bool                 m_lock = false;
        std::vector<Vector2> m_path;
    };

}  // namespace viz
