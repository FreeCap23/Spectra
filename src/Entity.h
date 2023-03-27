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
    int idx; // index of the hit object in the world vector

    inline void setFaceNormal(Ray ray, dvec3 outwardNormal) {
        frontFace = glm::dot(ray.getDirection(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Entity {
 public:
    Entity() {}

    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const = 0;

    virtual void setMat(std::shared_ptr<Material> mat) = 0;

    virtual std::shared_ptr<Material> getMat() const = 0;
};

class Sphere : public Entity {
 public:
    Sphere() {};
    Sphere(dvec3 center, double radius, std::shared_ptr<Material> mat_ptr) :
      m_center(center),
      m_radius(radius),
      m_mat_ptr(mat_ptr) {};
    dvec3 getCenter() const { return m_center; }
    double getRadius() const { return m_radius; }
    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const override;
    virtual void setMat(std::shared_ptr<Material> mat) override;
    virtual std::shared_ptr<Material> getMat() const override;

 private:
    dvec3 m_center;
    std::shared_ptr<Material> m_mat_ptr;
    double m_radius;
};

class Plane : public Entity {
 public:
    Plane() {};
    Plane(dvec3 normal, double distanceToOrigin, std::shared_ptr<Material> mat_ptr) :
        m_normal(normal),
        m_distanceToOrigin(distanceToOrigin),
        m_mat_ptr(mat_ptr) {}
    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const override;
    virtual void setMat(std::shared_ptr<Material> mat) override;
    virtual std::shared_ptr<Material> getMat() const override;

 private:
    dvec3 m_normal;
    double m_distanceToOrigin;
    std::shared_ptr<Material> m_mat_ptr;
};
