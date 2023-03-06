#pragma once

#include "glm/glm.hpp"

using glm::vec3;

struct Hits {
    float t_min;
    vec3 normal;
    vec3 intersection;
};

struct Ray {
    vec3 color;
    vec3 origin;
    vec3 direction;
    Hits hits;
};
