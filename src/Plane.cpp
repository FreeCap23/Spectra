#include "Plane.h"

Plane::Plane(vec3 norm, float dist) {
    normal = glm::normalize(norm);
    distance = dist;
}

bool Plane::hit(Ray& ray) {
	float angleWithPlane= glm::dot(normal, ray.direction);
	// Check if the ray intersects the plane
	if (angleWithPlane != 0) {
	    float t = -(glm::dot(normal, ray.origin) + distance) / angleWithPlane;
	    if (t > 0) {
	        ray.hits.intersection = ray.origin + ray.direction * t;
            ray.color = getColor(ray);
            ray.hits.t_min = t;
	        return true;
	    }
	}
    return false;
}

vec3 Plane::getColor(Ray ray) {
    // Calculate distance between intersection point and origin
    // We're only interested in the X and Y distances
    unsigned int distX = ray.hits.intersection.x;
    unsigned int distY = ray.hits.intersection.y;

    // Generate a checkerboard pattern
    if ((distX + distY) % 2 == 0)
        return vec3(53, 110, 252);
    else
        return vec3(127, 162, 250);
}
