#pragma once
#include <cstdint>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

namespace Spectra {
    inline double degrees_to_radians(double degrees) {
        return degrees * pi / 180.0;
    }
    inline double randomDouble() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator(1);
        return distribution(generator);
    }
    void writeImage(const char* fileName, int width, int height, uint8_t* data);
}
