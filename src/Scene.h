#pragma once
#include <memory>
#include <vector>
#include "Entity.h"

class Scene : public Entity {
 public:
    Scene() {}
    void clear() { objects.clear(); }
    void add(std::shared_ptr<Entity> obj) { objects.push_back(obj); }
    virtual bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const override;

 private:
    std::vector<std::shared_ptr<Entity>> objects;
};