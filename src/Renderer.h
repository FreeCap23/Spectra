#pragma once
#include <stdio.h>
#include <iostream>
#include <memory.h>
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

class Renderer {
 public:
    Renderer(Scene& scene, options& opts) : m_scene(scene), m_opts(opts) {}
    dvec3 getRayColor(Ray ray, int depth);
    double clamp(double value, double min, double max);
    void Render(uint8_t* data);
    void resetBuffer();

 public:
    unsigned int samplesDone = 0;

 private:
    options& m_opts;
    Scene& m_scene;
    std::vector<glm::dvec4> m_accumulation;
};