#include "Camera.h"
#include "Ray.h"
#include "Sphere.h"
#include "World.h"
#include "glm/glm.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <cstdio>
#include <cstdlib>
#include <random>

#define WIDTH 600
#define HEIGHT 400
#define SAMPLES 16 
#define MULTI_SAMPLE true

World world;

vec3 getColor(Ray& ray) {
    for (Sphere sphere : world.spheres) {
        if (sphere.hit(ray)) {
            vec3 intersectionPoint = ray.origin + ray.hits.t_min * ray.direction;
            vec3 normal = glm::normalize(intersectionPoint - sphere.center);

            // Map to [0, 1] and multiply by 255 to get a color
            vec3 normalColor = (normal + float(1.0)) / float(2.0) * float(255.0);
            return normalColor;
        }
    }
    // Return sky color if no intersection with any objects is found
    // This is the formula for the cosine of the two angles, so we have to map
    // it to the range [0, 1].
    float angleWithHorizon = glm::dot(ray.direction, vec3(0, 0, 1)) / glm::length(ray.direction);
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

    // Setup world
    // TODO: Fix having to put these in order
    world.spheres.push_back(Sphere(vec3(0, 1, 0), 0.5));
    world.spheres.push_back(Sphere(vec3(0, 4, 0), 2.5));

    // Calculate the size of one pixel
    float horizontalSize = 1.0 / (WIDTH);
    float verticalSize = 1.0 / (HEIGHT);

    // Setup random number generator
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> horizontalDistribution(-horizontalSize, horizontalSize);
    std::uniform_real_distribution<> verticalDistribution(-verticalSize, verticalSize);

    float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
    // Render loop
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // Setup ray
            Ray ray;
            ray.origin = camera.getPos();
            ray.direction.y = 1;

            #if MULTI_SAMPLE
            // Map pixel coordinates to ray direction
            float directionZ = i / (HEIGHT / 2.0) - 1.0;
            float directionX = (j / (WIDTH / 2.0) - 1.0) * aspectRatio;

            // Get color for this pixel by averaging multiple rays
            vec3 color(0, 0, 0);
            for (int s = 0; s < SAMPLES; s++) {
                ray.direction.z = directionZ + horizontalDistribution(generator);
                ray.direction.x = directionX + horizontalDistribution(generator);
                color += getColor(ray) * (float)(1.0/SAMPLES);
            }
            #else
            ray.direction.z = i / (HEIGHT / 2.0) - 1.0;
            ray.direction.x = (j / (WIDTH / 2.0) - 1.0) * aspectRatio;
            vec3 color = getColor(ray);
            #endif

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
