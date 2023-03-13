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
    auto materialGround = std::make_shared<Lambertian>(dvec3(0.8, 0.8, 0.8));
    auto materialCenter = std::make_shared<Lambertian>(dvec3(0.7, 0.3, 0.3));
    auto materialLeft = std::make_shared<Metal>(dvec3(0.8, 0.8, 0.8));
    auto materialRight = std::make_shared<Metal>(dvec3(0.8, 0.6, 0.2));
    Scene scene;
    scene.add(std::make_shared<Sphere>(dvec3(0, 1, 0), 0.5, materialCenter));
    scene.add(std::make_shared<Sphere>(dvec3(0, 1, -101.5), 100.5, materialGround));
    scene.add(std::make_shared<Sphere>(dvec3(1, 1, 0), 0.5, materialRight));
    scene.add(std::make_shared<Sphere>(dvec3(-1, 1, 0), 0.5, materialLeft));

    Camera camera(aspectRatio, dvec3(0));

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