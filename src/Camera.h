#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

using glm::dvec3;

class Camera {
 public:
   Camera() {}
   virtual Ray getRay(double u, double v) const = 0;
};

class Perspective : public Camera {
 public:
   Perspective() {}
   Perspective(
   double fov,
   double aspectRatio,
   dvec3 origin,
   dvec3 lookAt,
   dvec3 vectUp);

   virtual Ray getRay(double u, double v) const override;
 private:
   dvec3 m_origin, m_lowerLeftCorner, m_horizontal, m_vertical;
   double m_aspectRatio;
};

class Orthographic : public Camera {
 public:
   Orthographic() {}
   Orthographic(
      double scale,
      double aspectRatio,
      dvec3 origin,
      dvec3 lookAt,
      dvec3 vectUp);

   virtual Ray getRay(double u, double v) const override;
 private:
   dvec3 m_origin, m_horizontal, m_vertical, m_rayDirection; 
   double m_scale;
   double m_aspectRatio;
};