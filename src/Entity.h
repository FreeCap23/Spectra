#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "Ray.h"

using glm::vec3;

class Material;

struct hitRecord {
    dvec3 intersection, normal;
    std::shared_ptr<Material> mat_ptr;
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
