#pragma once
#include <memory>
#include <algorithm>
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
