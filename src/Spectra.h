#pragma once
#include <cstdint>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include "glm/glm.hpp"

using glm::dvec3;

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
    inline double randomDouble(double min, double max) {
        static std::uniform_real_distribution<double> distribution(min, max);
        static std::mt19937 generator(1);
        return distribution(generator);
    }

    inline static dvec3 randomVec() {
        return dvec3(randomDouble(), randomDouble(), randomDouble());
    }
    inline static dvec3 randomVec(double min, double max) {
        return dvec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
    }

    bool nearZero(dvec3 vec);

    dvec3 reflect(dvec3 vec, dvec3 normal);
    dvec3 refract(dvec3 vec, dvec3 normal, double refractionRatio);
    /**
     * Will return a random dvec3 inside a sphere of radius 1
    */
    dvec3 randomInUnitSphere();
    void writeImage(const char* fileName, int width, int height, const uint8_t* data);
}
