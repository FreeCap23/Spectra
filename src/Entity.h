#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

using glm::vec3;

struct hitRecord {
    dvec3 intersection, normal;
    double t;
    bool frontFace;

    inline void setFaceNormal(Ray ray, dvec3 outwardNormal) {
        frontFace = glm::dot(ray.getDirection(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Entity {
 public:
    Entity() {}

    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const = 0;
};
