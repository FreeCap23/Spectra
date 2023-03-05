#pragma once

#include "glm/glm.hpp"

using glm::vec3;

struct Hits {
    float t_min;
};

struct Ray {
    vec3 origin;
    vec3 direction;
    Hits hits;
};
