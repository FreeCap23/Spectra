#pragma once
#include "glm/glm.hpp"
#include "Sphere.h"
#include <vector>

using glm::vec3;

struct World {
    vec3 skyColor;
    std::vector<Sphere> spheres; 
};
