#include "Camera.h"
#include "Spectra.h"

Camera::Camera(
    double fov,
    double aspectRatio,
    dvec3 origin,
    dvec3 lookAt,
    dvec3 vectUp) :
    m_aspectRatio(aspectRatio), m_origin(origin) {
    double theta = Spectra::degrees_to_radians(fov);
    double h = tan(theta / 2.0);
    double vpHeight = 2.0 * h;
    double vpWidth = m_aspectRatio * vpHeight;

    dvec3 w = glm::normalize(m_origin - lookAt);
    dvec3 u = glm::normalize(glm::cross(vectUp, w));
    dvec3 v = glm::cross(w, u);

    double focalLength = 1.0;

    m_horizontal = vpWidth * u;
    m_vertical = vpHeight * v;
    m_lowerLeftCorner = origin - m_horizontal / 2.0 - m_vertical / 2.0 - w;

    /*
    m_horizontal = dvec3(vpWidth, 0, 0);
    m_vertical = dvec3(0, 0, vpHeight);
    m_lowerLeftCorner = 
        m_origin
        - m_horizontal / 2.0
        - m_vertical / 2.0
        + dvec3(0, focalLength, 0);
    */
}

Ray Camera::getRay(double u, double v) const {
    return Ray(
        m_origin,
        m_lowerLeftCorner + u*m_horizontal + v*m_vertical - m_origin);
}