#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

using glm::vec3;

class Entity {
    public:
        Entity() {};

        virtual bool hit(Ray&) = 0;
};
