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
    /**
     * Returns a random number in the (0, 1) range
    */
    inline double randomDouble() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator(1);
        return distribution(generator);
    }
    /**
     * Returns a random number in the given range
    */
    inline double randomDouble(double min, double max) {
        static std::uniform_real_distribution<double> distribution(min, max);
        static std::mt19937 generator(1);
        return distribution(generator);
    }

    /**
     * Returns a vector containing 3 random numbers in the (0, 1) range
    */
    inline static dvec3 randomVec() {
        return dvec3(randomDouble(), randomDouble(), randomDouble());
    }
    /**
     * Returns a vector containing 3 random numbers in the given range
    */
    inline static dvec3 randomVec(double min, double max) {
        return dvec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
    }

    /**
     * Returns true if any of the given vector's coordinates are close to 0
    */
    bool nearZero(dvec3 vec);

    /**
     * Will return a random dvec3 inside a sphere of radius 1
    */
    dvec3 randomInUnitSphere();

    /**
     * Writes given data buffer to a png file
    */
    void writeImage(const char* fileName, int width, int height, const uint8_t* data);
}
