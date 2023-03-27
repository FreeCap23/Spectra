#include "Scene.h"

void Scene::addSphere(std::shared_ptr<Material> mat, dvec3 center, double radius) {
    m_objects.push_back(std::make_shared<Sphere>(center, radius, mat));
}

void Scene::addPlane(std::shared_ptr<Material> mat, dvec3 normal, double distance) {
    m_objects.push_back(std::make_shared<Plane>(normal, distance, mat));
}

std::shared_ptr<Entity> Scene::getEntityAtIdx(int idx) {
    return m_objects[idx];
}

bool Scene::hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const {
    hitRecord tempRec;
    bool hitAnything = false;
    double t = t_max;

    // Loop through all objects in the scene & check for intersections
    for (int i = 0; i < m_objects.size(); i++) {
        if (m_objects[i]->hit(ray, t_min, t, tempRec)) {
            hitAnything = true;
            t = tempRec.t;
            hits = tempRec;
            hits.idx = i;
        }
    }

    return hitAnything;
}