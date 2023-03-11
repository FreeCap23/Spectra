#include "Sphere.h"

Sphere::Sphere(vec3 cen, float rad) {
    center = cen;
    radius = rad;
}

bool Sphere::hit(Ray& ray) {
    // h = b/2
    float a, h, c;
    a = glm::dot(ray.direction, ray.direction);
    h = glm::dot(ray.direction, (ray.origin - center));
    c = glm::dot((ray.origin - center), (ray.origin - center))
        - radius * radius;

    float determinant = h * h - a * c;
    // We compare to 0.01 to account for errors made by floating point
    // operations
    if (determinant > 0.01) {
        // We will get the shortest t for now
        float t1 = (-h - sqrt(determinant)) / a;
        float t2 = (-h + sqrt(determinant)) / a;
        ray.hits.t_min = t1 < t2 ? t1 : t2;
        ray.hits.intersection = ray.origin + ray.hits.t_min * ray.direction;
        ray.hits.normal = glm::normalize(ray.hits.intersection - center);
        // Map to [0, 1] and multiply by 255 to get a color
        ray.color = (ray.hits.normal + static_cast<float>(1.0))
            / static_cast<float>(2.0) * static_cast<float>(255.0);
        ray.color = vec3(1, 0, 0);
        return true;
    }
    return false;
}
