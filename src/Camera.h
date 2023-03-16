#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

using glm::dvec3;

class Camera {
 public:
    Camera() {}
    Camera(
      double fov,
      double aspectRatio,
      dvec3 origin,
      dvec3 lookAt,
      dvec3 vectUp);
    Ray getRay(double u, double v) const;

 private:
    dvec3 m_origin, m_lowerLeftCorner, m_horizontal, m_vertical;
    double m_aspectRatio;
};