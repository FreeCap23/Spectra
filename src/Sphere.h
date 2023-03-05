#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

using glm::vec3;

class Sphere {
    public:
        Sphere();
        Sphere(vec3 cen, float rad) {
            center = cen;
            radius = rad;
        }

        bool hit(Ray& ray) {        
            // h = b/2
            float a, h, c;
            a = glm::dot(ray.direction, ray.direction);
            h = glm::dot(ray.direction, (ray.origin - center));
            c = glm::dot((ray.origin - center), (ray.origin - center)) - radius * radius;
        
            float determinant = h * h - a * c;
            // We compare to 0.01 to account for errors made by floating point
            // operations
            if (determinant > 0.01) {
                // We will get the shortest t for now
                float t1 = (-h - sqrt(determinant)) / a;
                float t2 = (-h + sqrt(determinant)) / a;
                ray.hits.t_min = t1 < t2 ? t1 : t2;
                return true;
            }
            return false;
        }

    public:
        vec3 center;
        float radius;
};
