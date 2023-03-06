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
	        ray.color = (normal + (float)1.0 / (float)2.0 * (float)255.0);
	        return true;
	    }
	}
    return false;
}
