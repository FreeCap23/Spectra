#pragma once
#include "glm/glm.hpp"
#include "Ray.h"
#include "Entity.h"

using glm::vec3;

class Sphere : public Entity{
    public:
        Sphere();
        Sphere(vec3 cen, float rad);

        virtual bool hit(Ray& ray) override;

    public:
        vec3 center;
        float radius;
};
