#pragma once
#include "glm/glm.hpp"

using glm::dvec3;

class Ray {
 public:
    Ray() {}
    Ray(dvec3 orig, dvec3 dir) : origin(orig), direction(dir) {}
    dvec3 getOrigin() const { return origin; }
    dvec3 getDirection() const { return direction; }
    dvec3 at(double t) { return origin + t * direction; }

 private:
    dvec3 origin, direction;
};