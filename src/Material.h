#pragma once
#include "Entity.h"
#include "Spectra.h"

struct hitRecord;

class Material {
 public:
    virtual bool scatter(
        Ray ray,
        hitRecord hits,
        dvec3& attenuation,
        Ray& scattered)
        const = 0;
};

class Lambertian : public Material {
 public:
    Lambertian(dvec3 albedo) : m_albedo(albedo) {}
    virtual bool scatter(
        Ray ray,
        hitRecord hits,
        dvec3& attenuation,
        Ray& scattered)
    const override {
        dvec3 scatterDirection = hits.normal + Spectra::randomInUnitSphere();
        // If the vector is near zero that means the normal and the target vectors
        // cancelled each other out. To remedy this, just set the scatterDirection
        // vector to the normal vector.
        if (Spectra::nearZero(scatterDirection))
            scatterDirection = hits.normal;
        scattered = Ray(hits.intersection, scatterDirection);
        attenuation = m_albedo;
        return true;
    }

 private:
    dvec3 m_albedo;
};

class Metal : public Material {
 public:
    Metal(dvec3 albedo) : m_albedo(albedo) {}
    virtual bool scatter(
        Ray ray,
        hitRecord hits,
        dvec3& attenuation,
        Ray& scattered)
    const override {
        dvec3 reflected = Spectra::reflect(glm::normalize(ray.getDirection()), hits.normal);
        scattered = Ray(hits.intersection, reflected);
        attenuation = m_albedo;
        return glm::dot(reflected, hits.normal) > 0;
    }

 private:
    dvec3 m_albedo;
};