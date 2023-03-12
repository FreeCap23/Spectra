#include "Scene.h"

bool Scene::hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const {
    hitRecord tempRec;
    bool hitAnything = false;
    double t = t_max;

    for (std::shared_ptr<Entity> object : objects) {
        if (object->hit(ray, t_min, t, tempRec)) {
            hitAnything = true;
            t = tempRec.t;
            hits = tempRec;
        }
    }

    return hitAnything;
}