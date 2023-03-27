#pragma once
#include <memory>
#include <vector>
#include "Entity.h"
#include "Camera.h"

class Scene {
 public:
    Scene() {}
    void clear() { m_objects.clear(); }
    void setCamera(std::shared_ptr<Camera> cam) { camera = cam; }
    void add(std::shared_ptr<Entity> obj) { m_objects.push_back(obj); }
    void addSphere(std::shared_ptr<Material> mat, dvec3 center, double radius);
    void addPlane(std::shared_ptr<Material> mat, dvec3 normal, double distance);
    std::shared_ptr<Entity> getEntityAtIdx(int idx);
    int getEntityCount();
    bool hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const;

 public:
   std::shared_ptr<Camera> camera;

 private:
    std::vector<std::shared_ptr<Entity>> m_objects;
};