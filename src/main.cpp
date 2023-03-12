#include <stdio.h>
#include "glm/glm.hpp"
#include "Spectra.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Spectra.h"
#include "Camera.h"

using glm::dvec3;

struct options {
    bool multisample;
    int samples;
    int width;
    int height;
};

dvec3 getRayColor(Ray ray, Scene scene) {
    hitRecord hits;
    if (scene.hit(ray, 0, infinity, hits)) {
        return (hits.normal + 1.0) / 2.0;
    }
    return dvec3(0, 0, 0);
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
    opts.samples = 16;
    const double aspectRatio = static_cast<double>(opts.width) / opts.height;

    /*
     * Initialize scene
     */
    Scene scene;
    scene.add(std::make_shared<Sphere>(dvec3(0, 1, 0), 0.5));
    scene.add(std::make_shared<Sphere>(dvec3(0, 1, -101.5), 100.5));

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
                color += getRayColor(ray, scene);
            }
            color = color / static_cast<double>(opts.samples);
            data[offset++] = static_cast<int>(clamp(color.x, 0, 1) * 255);
            data[offset++] = static_cast<int>(clamp(color.y, 0, 1) * 255);
            data[offset++] = static_cast<int>(clamp(color.z, 0, 1) * 255);
        }
    }
    Spectra::writeImage("out.png", opts.width, opts.height, data);
    printf("Done rendering.\n");
    return 0;
}