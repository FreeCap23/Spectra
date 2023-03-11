#include <cstdio>
#include <cstdlib>
#include <random>
#include <limits>
#include "Camera.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "World.h"
#include "Spectra.h"
#include "glm/glm.hpp"

#define WIDTH 600
#define HEIGHT 400
#define SAMPLES 16
#define MULTI_SAMPLE true

World world;

vec3 getColor(Ray& ray) {
    // Return sky color if no intersection with any objects is found
    // This is the formula for the cosine of the two angles, so we have to map
    // it to the range [0, 1].
    float angleWithHorizon =
        glm::dot(ray.direction, vec3(0, 0, 1))
        / glm::length(ray.direction);
    angleWithHorizon = (angleWithHorizon + 1) / 2.0;
    vec3 color = vec3(0.58, 0.86, 1) * angleWithHorizon;

    float t_min = std::numeric_limits<float>::infinity();
    for (std::unique_ptr<Entity> &entity : world.entities) {
        if (entity->hit(ray) && ray.hits.t_min <= t_min) {
            t_min = ray.hits.t_min;
            // TODO: See if the ray is intersected and don't add
            // the light if yes.
            color = ray.color;
            float brightnessAtPoint = 0;
            vec3 lightsColor(0, 0, 0);
            for (std::unique_ptr<Light> &light : world.lights) {
                // Calculate angle between the normal and the light
                vec3 vectorToLight = ray.hits.intersection - light->position;
                float angle = glm::dot(ray.hits.normal, vectorToLight);
                angle = (-angle + 1) / 2.0;
                // Get the distance to the light
                float distToLight = glm::abs(glm::length(vectorToLight));
                // Calculate brightness at the intersection point
                brightnessAtPoint += (light->brightness * (1.0 / (distToLight * distToLight))) * angle;
                // Add the light's color to the ray color
                lightsColor += brightnessAtPoint * light->color;
            }
            color *= lightsColor;
        }
    }
    return color;
}

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
    world.entities.emplace_back(new Plane(vec3(0, 0, 1), 2.0));
    world.entities.emplace_back(new Sphere(vec3(-0.1, 2, 0.3), 0.7));
    world.entities.emplace_back(new Sphere(vec3(0.1, 1, 0), 0.2));
    // Setup Light properties
    vec3 lightColor(0.39, 0.39, 0.39);
    vec3 lightPos(-1.5, 1, 1);
    float lightBright = 2.8;
    world.lights.emplace_back(new Light(lightPos, lightColor, lightBright));

    // Calculate the size of one pixel
    float horizontalSize = 1.0 / (WIDTH);
    float verticalSize = 1.0 / (HEIGHT);

    // Setup random number generator
    std::mt19937 generator(1);
    std::uniform_real_distribution<>
        horizontalDistribution(-horizontalSize, horizontalSize);
    std::uniform_real_distribution<>
        verticalDistribution(-verticalSize, verticalSize);

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
                ray.direction.z = directionZ
                    + verticalDistribution(generator);
                ray.direction.x = directionX
                    + horizontalDistribution(generator);
                color += getColor(ray) * static_cast<float>(1.0/SAMPLES);
            }
            #else
            ray.direction.z = i / (HEIGHT / 2.0) - 1.0;
            ray.direction.x = (j / (WIDTH / 2.0) - 1.0) * aspectRatio;
            vec3 color = getColor(ray);
            #endif

            // This is pretty stupid but it works
            if (color.x > 1)
                color.x = 1;
            if (color.y > 1)
                color.y = 1;
            if (color.z > 1)
                color.z = 1;
            if (color.x < 0)
                color.x = 0;
            if (color.y < 0)
                color.y = 0;
            if (color.z < 0)
                color.z = 0;

            // Write the color to the image buffer
            data[offset++] = static_cast<uint8_t>(color.x * 255.0);
            data[offset++] = static_cast<uint8_t>(color.y * 255.0);
            data[offset++] = static_cast<uint8_t>(color.z * 255.0);
        }
    }

    Spectra::writeImage("out.png", WIDTH, HEIGHT, data);

    return 0;
}
