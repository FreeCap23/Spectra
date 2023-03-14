#pragma once
#include <stdio.h>
#include <iostream>
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
    Renderer() {}
    dvec3 getRayColor(Ray ray, Scene scene, int depth);
    double clamp(double value, double min, double max);
    void Render();
};