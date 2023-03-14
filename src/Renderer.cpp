#include "Renderer.h"

dvec3 Renderer::getRayColor(Ray ray, int depth) {
    if (depth <= 0) {
        return dvec3(0);
    }

    hitRecord hits;
    if (m_scene.hit(ray, 0.001, infinity, hits)) {
        Ray scattered;
        dvec3 attenuation;
        if (hits.mat_ptr->scatter(ray, hits, attenuation, scattered))
            return attenuation * getRayColor(scattered, depth - 1);
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

void Renderer::Initialize(options opts, uint8_t* data) {
    m_opts = opts;
    /*
     * Render options
     */
    const double aspectRatio = static_cast<double>(opts.width) / opts.height;

    /*
     * Initialize scene
     */
    auto matGround = std::make_shared<Lambertian>(dvec3(0.2, 0.8, 0.5));
    auto matMirror = std::make_shared<Metal>(dvec3(1, 1, 1), 0.02);

    m_scene.add(std::make_shared<Plane>(dvec3(0, 0, 1), 0, matGround));
    m_scene.add(std::make_shared<Plane>(dvec3(0, -1, 0), 3, matMirror));

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
            m_scene.add(std::make_shared<Sphere>(position, 0.5, mat));
        }
    }
    m_camera = Camera(8, aspectRatio, dvec3(0, -50, 35), dvec3(0, 0, 2), dvec3(0, 0, 1));
}

void Renderer::Render(uint8_t* data) {
    int offset = 0;
    static dvec3 color;
    for (int i = m_opts.height; i > 0; i--) {
        std::cerr << "\r" << i-1 << " lines remaining.";
        for (int j = 0; j < m_opts.width; j++) {
            double u, v;
            u = (j + Spectra::randomDouble()) / (m_opts.width - 1);
            v = (i + Spectra::randomDouble()) / (m_opts.height - 1);
            Ray ray = m_camera.getRay(u, v);
            color = getRayColor(ray, m_opts.maxDepth);
            color = (color / static_cast<double>(m_opts.samples));
            data[offset++] += static_cast<int>(clamp(color.x, 0, 1) * 255);
            data[offset++] += static_cast<int>(clamp(color.y, 0, 1) * 255);
            data[offset++] += static_cast<int>(clamp(color.z, 0, 1) * 255);
        }
    }
}