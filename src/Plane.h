#pragma once
#include "glm/glm.hpp"
#include "Entity.h"

using glm::vec3;

class Plane : public Entity {
public:
    Plane();
    Plane(vec3 norm, float dist);

    virtual bool hit(Ray& ray) override;

    vec3 getColor(Ray ray);

public:
    vec3 normal;
    float distance;
};
