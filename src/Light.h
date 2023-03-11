#pragma once

#include "glm/glm.hpp"
using glm::vec3;

class Light {
 public:
    Light() {}

    Light(vec3 pos, vec3 col, float bright);
 public:
    vec3 position, color;
    float brightness;
};
