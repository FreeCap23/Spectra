#include "Renderer.h"

dvec3 Renderer::getRayColor(Ray ray, Scene scene, int depth) {
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
double Renderer::clamp(double value, double min, double max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}
void Renderer::Render() {
    /*
     * Render options
     */
    options opts;
    opts.width = 2560;
    opts.height = 1600;
    opts.samples = 500;
    opts.maxDepth = 100;
    const double aspectRatio = static_cast<double>(opts.width) / opts.height;

    /*
     * Initialize scene
     */
    auto matGround = std::make_shared<Lambertian>(dvec3(0.2, 0.8, 0.5));
    auto matMirror = std::make_shared<Metal>(dvec3(1, 1, 1), 0.02);

    Scene scene;
    scene.add(std::make_shared<Plane>(dvec3(0, 0, 1), 0, matGround));
    scene.add(std::make_shared<Plane>(dvec3(0, -1, 0), 3, matMirror));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            double picker = Spectra::randomDouble();
            std::shared_ptr<Material> mat;
            if (picker < 0.33) {
                dvec3 color = Spectra::randomVec();
                mat = std::make_shared<Lambertian>(color);
            } else if (picker < 0.66) {
                dvec3 color = Spectra::randomVec();
                double roughness = Spectra::randomDouble();
                mat = std::make_shared<Metal>(color, roughness);
            } else {
                double ior = Spectra::randomDouble();
                mat = std::make_shared<Dielectric>(ior);
            }
            dvec3 position((j-3.5) * 1.25, (i-1.5) * 1.25, 0.5);
            scene.add(std::make_shared<Sphere>(position, 0.5, mat));
        }
    }

    Camera camera(8, aspectRatio, dvec3(0, -50, 35), dvec3(0, 0, 2), dvec3(0, 0, 1));

    /*
     * Initialize output file  
     */
    uint8_t* data = new uint8_t[3 * opts.width * opts.height];
    int offset = 0;
    
    /*
     * Render loop
     */
    printf("Starting render...");
    for (int i = 0; i < opts.height; i++) {
        std::cerr << "\r" << opts.height - i << " lines remaining.";
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
    printf("\r\n");
}