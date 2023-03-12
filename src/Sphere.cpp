#include "Sphere.h"

bool Sphere::hit(Ray& ray, double t_min, double t_max, hitRecord& hits) const {
    // h = b/2
    float a, h, c;
    a = glm::dot(ray.getDirection(), ray.getDirection());
    h = glm::dot(ray.getDirection(), (ray.getOrigin() - center));
    c = glm::dot((ray.getOrigin() - center), (ray.getOrigin() - center))
        - radius * radius;
    float determinant = h * h - a * c;
    // We compare to 0.01 to account for errors made by floating point
    // operations
    if (determinant < 0) {
        return false;
    } else {
        // We will get the shortest t for now
        float t1 = (-h - sqrt(determinant)) / a;
        float t2 = (-h + sqrt(determinant)) / a;

        double t = std::min(t1, t2);
        if (t < t_min || t > t_max) {
            return false;
        }
        hits.t = t;
        hits.intersection = ray.at(t);
        dvec3 outwardNormal = glm::normalize((hits.intersection - center) / radius);
        hits.setFaceNormal(ray, outwardNormal);
        return true;
    }
}
