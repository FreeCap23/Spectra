#include "Spectra.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Spectra::writeImage(
        const char *fileName,
        int width,
        int height,
        uint8_t *data) {
    // Flip image vertically
    stbi_flip_vertically_on_write(1);

    // Write image buffer to file
    stbi_write_png(fileName, width, height, 3, data, 3 * width);
}

dvec3 Spectra::randomInUnitSphere() {
    while (true) {
        dvec3 p = randomVec(-1, 1);
        if (glm::length(p) * glm::length(p) >= 1)
            continue; // skip this value and generate another one
        return glm::normalize(p);
    }
}