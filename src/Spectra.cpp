#include "Spectra.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

dvec3 Spectra::reflect(dvec3 vec, dvec3 normal) {
    return vec - 2 * glm::dot(vec, normal) * normal;
}

dvec3 Spectra::refract(dvec3 vec, dvec3 normal, double refractionRatio) {
    double cosTheta = fmin(glm::dot(vec, normal), 1.0);
    dvec3 dirPerpendicular = refractionRatio * (vec * cosTheta * normal);
    double dirPLengthSquared = glm::length(dirPerpendicular) * glm::length(dirPerpendicular);
    dvec3 dirParallel = -sqrt(fabs(1.0 - dirPLengthSquared)) * normal;
    return dirPerpendicular + dirParallel;
}