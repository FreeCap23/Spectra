#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

using glm::dvec3;

class Camera {
 public:
   Camera() {}
   virtual Ray getRay(double u, double v) = 0;
};

class Perspective : public Camera {
 public:
   Perspective(
   float& fov,
   double aspectRatio,
   dvec3& origin,
   dvec3& lookAt,
   dvec3 vectUp);

   virtual Ray getRay(double u, double v) override;
 private:
   dvec3& m_origin;
   dvec3& m_lookAt;
   dvec3 m_lowerLeftCorner, m_horizontal, m_vertical, m_vectUp;
   double m_aspectRatio;
   float& m_fov;
};

class Orthographic : public Camera {
 public:
   Orthographic(
      float& scale,
      double aspectRatio,
      dvec3& origin,
      dvec3& lookAt,
      dvec3 vectUp);

   virtual Ray getRay(double u, double v) override;
 private:
   dvec3& m_origin;
   dvec3& m_lookAt;
   dvec3 m_horizontal, m_vertical, m_rayDirection, m_vectUp; 
   float& m_scale;
   double m_aspectRatio;
};