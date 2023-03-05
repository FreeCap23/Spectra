#include "Camera.h"
#include "Ray.h"
#include "glm/glm.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <cstdio>
#include <cstdlib>

#define WIDTH 400
#define HEIGHT 200

vec3 getColor(Ray& ray) {
    // Return sky color if no intersection with any objects is found
    // This is the formula for the cosine of the two angles, so we have to map
    // it to the range [0, 1].
    float angleWithHorizon = glm::dot(ray.direction, vec3(0, 1, 0)) / glm::length(ray.direction);
    angleWithHorizon = (-angleWithHorizon + 1) / 2.0;
    return vec3(150, 220, 255) * angleWithHorizon;
}

// TODO: Add argument for file name
int main() {
    // Allocate memory for the data
    // 3 means 3 channels of 8 bits each, so 1 byte each
    // 3 bytes for every pixel in the image
    // There are WIDTH * HEIGHT pixels in the image
    uint8_t* data = new uint8_t[3 * WIDTH * HEIGHT];
    int offset = 0;

    // Setup camera
    Camera camera(vec3(0, 0, 0));

    // Setup ray
    Ray ray;
    ray.origin = camera.getPos();
    ray.direction.z = 1;

    // Render loop
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // Map pixel coordinates to ray direction
            ray.direction.y = i / (HEIGHT / 2.0) - 1.0;
            ray.direction.x = j / (WIDTH / 2.0) - 1.0;

            // Get color for this pixel
            vec3 color = getColor(ray);

            // Write the color to the image buffer
            data[offset++] = static_cast<uint8_t>(color.x);
            data[offset++] = static_cast<uint8_t>(color.y);
            data[offset++] = static_cast<uint8_t>(color.z);
        }
    }

    // Write image buffer to file
    stbi_write_png("out.png", WIDTH, HEIGHT, 3, data, 3 * WIDTH);

    free(data);

    return 0;
}
