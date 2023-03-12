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