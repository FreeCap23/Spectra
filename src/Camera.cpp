#include "Camera.h"

Camera::Camera(double aspectRatio, dvec3 origin) :
    m_aspectRatio(aspectRatio), m_origin(origin) {
    double vpHeight = 2.0;
    double vpWidth = m_aspectRatio * vpHeight;
    double focalLength = 1.0;

    m_horizontal = dvec3(vpWidth, 0, 0);
    m_vertical = dvec3(0, 0, vpHeight);
    m_lowerLeftCorner = 
        m_origin
        - m_horizontal / 2.0
        - m_vertical / 2.0
        + dvec3(0, focalLength, 0);
}

Ray Camera::getRay(double u, double v) const {
    return Ray(
        m_origin,
        m_lowerLeftCorner + u*m_horizontal + v*m_vertical - m_origin);
}