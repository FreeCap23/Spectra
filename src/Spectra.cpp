#include "Spectra.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

double Spectra::randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator(1);
    return distribution(generator);
}

double Spectra::randomDouble(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator(1);
    return distribution(generator);
}

dvec3 Spectra::randomVec() {
    return dvec3(randomDouble(), randomDouble(), randomDouble());
}

dvec3 Spectra::randomVec(double min, double max) {
    return dvec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

void Spectra::writeImage(
        const char *fileName,
        int width,
        int height,
        const uint8_t *data) {

    // Write image buffer to file
    stbi_write_png(fileName, width, height, 4, data, 4 * width);
}

dvec3 Spectra::randomInUnitSphere() {
    while (true) {
        dvec3 p = randomVec(-1, 1);
        if (glm::length(p) * glm::length(p) >= 1)
            continue; // skip this value and generate another one
        return glm::normalize(p);
    }
}

bool Spectra::nearZero(dvec3 vec) {
    const double limit = 1e-8;
    return (fabs(vec.x) < limit && fabs(vec.y) < limit && fabs(vec.z) < limit);
}