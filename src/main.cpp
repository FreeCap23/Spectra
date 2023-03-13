#include <stdio.h>
#include "Spectra.h"
#include "Ray.h"
#include "Scene.h"
#include "Spectra.h"
#include "Camera.h"
#include "Material.h"
#include "Entity.h"

struct options {
    bool multisample;
    int samples;
    int maxDepth;
    int width;
    int height;
};

dvec3 getRayColor(Ray ray, Scene scene, int depth) {
    if (depth <= 0) {
        return dvec3(0);
    }

    hitRecord hits;
    if (scene.hit(ray, 0.001, infinity, hits)) {
        Ray scattered;
        dvec3 attenuation;
        if (hits.mat_ptr->scatter(ray, hits, attenuation, scattered))
            return attenuation * getRayColor(scattered, scene, depth - 1);
        return dvec3(0);
    }

    double angleWithHorizon =
        glm::dot(ray.getDirection(), dvec3(0, 0, 1))
        / glm::length(ray.getDirection());
    angleWithHorizon = (angleWithHorizon + 1) / 2.0;
    return (1.0-angleWithHorizon)*dvec3(1.0, 1.0, 1.0) + angleWithHorizon*dvec3(0.5, 0.7, 1.0);
}

double clamp(double value, double min, double max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

int main() {
    /*
     * Render options
     */
    options opts;
    opts.width = 600;
    opts.height = 400;
    opts.multisample = true;
    opts.samples = 50;
    opts.maxDepth = 50;
    const double aspectRatio = static_cast<double>(opts.width) / opts.height;

    /*
     * Initialize scene
     */
    // Ground
    auto matGround = std::make_shared<Lambertian>(dvec3(0.2, 0.8, 0.5));
    dvec3 posGround(0, 1, -101.5);
    double radGround = 100.5;
    // Lower left corner
    auto matLLC = std::make_shared<Dielectric>(1.6);
    dvec3 posLLC(-0.5, 0.5, -0.5);
    double radLLC = 0.5;
    // Upper left corner
    auto matULC = std::make_shared<Metal>(dvec3(0.7, 0.7, 0.7), 0.7);
    dvec3 posULC(-0.5, 0.5, 0.5);
    double radULC = 0.5;
    // Lower right corner
    auto matLRC = std::make_shared<Lambertian>(dvec3(0.6, 0.01, 0.6));
    dvec3 posLRC(0.5, 0.5, -0.5);
    double radLRC = 0.5;
    // Upper right corner
    auto matURC = std::make_shared<Metal>(dvec3(0.1, 0.3, 0.6), 0.1);
    dvec3 posURC(0.5, 0.5, 0.5);
    double radURC = 0.5;
    Scene scene;
    scene.add(std::make_shared<Sphere>(posGround, radGround, matGround));
    scene.add(std::make_shared<Sphere>(posLLC, radLLC, matLLC));
    scene.add(std::make_shared<Sphere>(posULC, radULC, matULC));
    scene.add(std::make_shared<Sphere>(posLRC, radLRC, matLRC));
    scene.add(std::make_shared<Sphere>(posURC, radURC, matURC));

    Camera camera(12, aspectRatio, dvec3(0, -10, 0), dvec3(0, 1, 0), dvec3(0, 0, 1));

    /*
     * Initialize output file  
     */
    uint8_t* data = new uint8_t[3 * opts.width * opts.height];
    int offset = 0;
    
    /*
     * Render loop
     */
    printf("Starting render...\n");
    for (int i = 0; i < opts.height; i++) {
        for (int j = 0; j < opts.width; j++) {
            dvec3 color(0);
            for (int s = 0; s < opts.samples; s++) {
                double u, v;
                u = (j + Spectra::randomDouble()) / (opts.width - 1);
                v = (i + Spectra::randomDouble()) / (opts.height - 1);
                Ray ray = camera.getRay(u, v);
                color += getRayColor(ray, scene, opts.maxDepth);
            }
            color = glm::sqrt(color / static_cast<double>(opts.samples));
            data[offset++] = static_cast<int>(clamp(color.x, 0, 1) * 255);
            data[offset++] = static_cast<int>(clamp(color.y, 0, 1) * 255);
            data[offset++] = static_cast<int>(clamp(color.z, 0, 1) * 255);
        }
    }
    Spectra::writeImage("../out.png", opts.width, opts.height, data);
    printf("Done rendering.\n");
    return 0;
}