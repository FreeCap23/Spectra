#include "Renderer.h"

dvec3 Renderer::getRayColor(Ray ray, int depth) {
    // Early return if recursion depth hit its limit
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

    // If no object in the scene has been hit, calculate sky color
    double angleWithHorizon = glm::dot(ray.getDirection(), dvec3(0, 0, 1)) / glm::length(ray.getDirection());
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

void Renderer::Render(uint8_t* data) {
    int offset = 0;
    int accOffset = 0;
    dvec3 color;
    for (int i = m_opts.height; i > 0; i--) {
        std::cerr << "\r" << i-1 << "\t lines remaining.";
        for (int j = 0; j < m_opts.width; j++) {
            double u, v;
            u = (j + Spectra::randomDouble()) / (m_opts.width - 1);
            v = (i + Spectra::randomDouble()) / (m_opts.height - 1);
            Ray ray = m_scene.camera->getRay(u, v);
            color = getRayColor(ray, m_opts.maxDepth);
            m_accumulation[accOffset] += glm::dvec4(color, 1);
            data[offset++] = static_cast<uint8_t>(glm::sqrt(m_accumulation[accOffset].x / samplesDone) * 255);
            data[offset++] = static_cast<uint8_t>(glm::sqrt(m_accumulation[accOffset].y / samplesDone) * 255);
            data[offset++] = static_cast<uint8_t>(glm::sqrt(m_accumulation[accOffset].z / samplesDone) * 255);
            data[offset++] = 255; // Alpha channel
            accOffset++;
        }
    }
}

void Renderer::resetBuffer() {
    m_accumulation.assign(m_opts.width * m_opts.height, glm::dvec4(0));
    samplesDone = 0;
}