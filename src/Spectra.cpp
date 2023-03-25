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