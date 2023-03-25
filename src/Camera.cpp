#include "Camera.h"
#include "Spectra.h"

Perspective::Perspective(
    double fov,
    double aspectRatio,
    dvec3 origin,
    dvec3 lookAt,
    dvec3 vectUp) :
    m_aspectRatio(aspectRatio), m_origin(origin) {
    double theta = glm::radians(fov);
    double h = tan(theta / 2.0);
    double vpHeight = 2.0 * h;
    double vpWidth = m_aspectRatio * vpHeight;

    dvec3 w = glm::normalize(m_origin - lookAt);
    dvec3 u = glm::normalize(glm::cross(vectUp, w));
    dvec3 v = glm::cross(w, u);

    m_horizontal = vpWidth * u;
    m_vertical = vpHeight * v;
    m_lowerLeftCorner = origin - m_horizontal / 2.0 - m_vertical / 2.0 - w;
}

Ray Perspective::getRay(double u, double v) const {
    return Ray(
        m_origin,
        m_lowerLeftCorner + u*m_horizontal + v*m_vertical - m_origin);
}

Orthographic::Orthographic(
    double scale,
    double aspectRatio,
    dvec3 origin,
    dvec3 lookAt,
    dvec3 vectUp) :
    m_scale(scale), m_aspectRatio(aspectRatio), m_origin(origin)
{
    double vpHeight = m_scale;
    double vpWidth = m_aspectRatio * vpHeight;

    dvec3 w = glm::normalize(m_origin - lookAt);
    dvec3 u = glm::normalize(glm::cross(vectUp, w));
    dvec3 v = glm::cross(w, u);

    m_rayDirection = -w;

    m_horizontal = vpWidth * u;
    m_vertical = vpHeight * v;
}

Ray Orthographic::getRay(double u, double v) const {
    // Make u & v range from (-1, 1)
    u = u*2 - 1;
    v = v*2 - 1;
    // Offset the ray's origin based on the u & v coordinates
    return Ray(m_origin + m_horizontal * u + m_vertical * v, m_rayDirection);
}