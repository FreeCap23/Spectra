#pragma once
#include <memory>
#include <vector>
#include "Entity.h"

class Scene : public Entity {
 public:
    Scene() {}
    void clear() { m_objects.clear(); }
    void add(std::shared_ptr<Entity> obj) { m_objects.push_back(obj); }
    void addSphere(std::shared_ptr<Material> mat, dvec3 center, double radius);
    void addPlane(std::shared_ptr<Material> mat, dvec3 normal, double distance);
    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const override;

 private:
    std::vector<std::shared_ptr<Entity>> m_objects;
};