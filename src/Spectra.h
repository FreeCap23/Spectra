#pragma once
#include <cstdint>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include "glm/glm.hpp"

using glm::dvec3;

const double infinity = std::numeric_limits<double>::infinity();

namespace Spectra {
    /**
     * Returns a random number in the (0, 1) range
    */
    double randomDouble();

    /**
     * Returns a random number in the given range
    */
    double randomDouble(double min, double max);

    /**
     * Returns a vector containing 3 random numbers in the (0, 1) range
    */
    static dvec3 randomVec();

    /**
     * Returns a vector containing 3 random numbers in the given range
    */
    static dvec3 randomVec(double min, double max);

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
