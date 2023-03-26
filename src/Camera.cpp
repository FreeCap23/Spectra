#include "Camera.h"
#include "Spectra.h"

Perspective::Perspective(
    float& fov,
    double aspectRatio,
    dvec3& origin,
    dvec3& lookAt,
    dvec3 vectUp) :
    m_fov(fov), m_aspectRatio(aspectRatio), m_origin(origin), m_lookAt(lookAt), m_vectUp(vectUp) {
}

Ray Perspective::getRay(double u, double v) {
    double theta = glm::radians(m_fov);
    double h = tan(theta / 2.0);
    double vpHeight = 2.0 * h;
    double vpWidth = m_aspectRatio * vpHeight;

    dvec3 w1 = glm::normalize(m_origin - m_lookAt);
    dvec3 u1 = glm::normalize(glm::cross(m_vectUp, w1));
    dvec3 v1 = glm::cross(w1, u1);

    m_horizontal = vpWidth * u1;
    m_vertical = vpHeight * v1;
    m_lowerLeftCorner = m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - w1;

    return Ray(
        m_origin,
        m_lowerLeftCorner + u*m_horizontal + v*m_vertical - m_origin);
}

Orthographic::Orthographic(
    float& scale,
    double aspectRatio,
    dvec3& origin,
    dvec3& lookAt,
    dvec3 vectUp) :
    m_scale(scale), m_aspectRatio(aspectRatio), m_origin(origin), m_lookAt(lookAt), m_vectUp(vectUp)
{
}

Ray Orthographic::getRay(double u, double v) {
    double vpHeight = m_scale / 2.0;
    double vpWidth = m_aspectRatio * vpHeight;

    dvec3 w1 = glm::normalize(m_origin - m_lookAt);
    dvec3 u1 = glm::normalize(glm::cross(m_vectUp, w1));
    dvec3 v1 = glm::cross(w1, u1);

    m_rayDirection = -w1;

    m_horizontal = vpWidth * u1;
    m_vertical = vpHeight * v1;

    // Make u & v range from (-1, 1)
    u = u*2 - 1;
    v = v*2 - 1;
    // Offset the ray's origin based on the u & v coordinates
    return Ray(m_origin + m_horizontal * u + m_vertical * v, m_rayDirection);
}