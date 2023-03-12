#pragma once
#include <algorithm>
#include "glm/glm.hpp"
#include "Ray.h"
#include "Entity.h"

using glm::dvec3;

class Sphere : public Entity{
 public:
    Sphere() {};
    Sphere(dvec3 center, double radius, std::shared_ptr<Material> mat_ptr) :
      m_center(center),
      m_radius(radius),
      m_mat_ptr(mat_ptr) {};
    dvec3 getCenter() const { return m_center; }
    double getRadius() const { return m_radius; }

    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const override;

 private:
    dvec3 m_center;
    std::shared_ptr<Material> m_mat_ptr;
    double m_radius;
};
