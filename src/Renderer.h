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
    Renderer() {};
    dvec3 getRayColor(Ray ray, int depth);
    double clamp(double value, double min, double max);
    void Initialize(options opts);
    void Render(uint8_t* data);

 private:
    options m_opts;
    std::shared_ptr<Camera> m_camera;
    Scene m_scene;
};