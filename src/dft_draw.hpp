#pragma once

#include <complex>
#include <tuple>
#include <vector>

#include "raylib.h"

using Complex   = std::complex<double>;
using DFTResult = std::vector<std::tuple<Complex, double, double, double>>;

struct Epicycloid {
    Epicycloid(double amp, double phase, double freq);

    double radius;
    double theta;
    double freq;
};

DFTResult computeDFT(const std::vector<Complex>& samples);
std::vector<Epicycloid> createEpicycloids(const DFTResult& dftResult);
std::vector<Complex> readComplexNumbers(const std::string& filename);
std::vector<Complex> getUserPath();
std::vector<Complex> getUserPathInExistingWindow();
void runVisualizationInExistingWindow(const std::vector<Epicycloid>& epicycloids, size_t sampleCount);

class DFTVisualizer {
  public:
    DFTVisualizer(int width, int height, const char* title);
    ~DFTVisualizer();

    void run(const std::vector<Epicycloid>& epicycloids, size_t sampleCount);

  private:
    void drawFrame(const std::vector<Epicycloid>& epicycloids);

    int m_width;
    int m_height;
    double m_time = 0.0f;
    double m_zoom = 1.0f;
    bool m_lock   = false;

    std::vector<Vector2> m_path;
};
