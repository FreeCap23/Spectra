#pragma once
#include <algorithm>
#include "glm/glm.hpp"
#include "Ray.h"
#include "Entity.h"

using glm::dvec3;

class Sphere : public Entity{
 public:
    Sphere() {};
    Sphere(dvec3 cen, double rad) : center(cen), radius(rad) {};
    dvec3 getCenter() const { return center; }
    double getRadius() const { return radius; }

    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const override;

 private:
    dvec3 center;
    double radius;
};
