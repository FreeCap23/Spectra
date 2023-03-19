#include "Scene.h"

void Scene::addSphere(std::shared_ptr<Material> mat, dvec3 center, double radius) {
    m_objects.push_back(std::make_shared<Sphere>(center, radius, mat));
}

void Scene::addPlane(std::shared_ptr<Material> mat, dvec3 normal, double distance) {
    m_objects.push_back(std::make_shared<Plane>(normal, distance, mat));
}

bool Scene::hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const {
    hitRecord tempRec;
    bool hitAnything = false;
    double t = t_max;

    for (std::shared_ptr<Entity> object : m_objects) {
        if (object->hit(ray, t_min, t, tempRec)) {
            hitAnything = true;
            t = tempRec.t;
            hits = tempRec;
        }
    }

    return hitAnything;
}