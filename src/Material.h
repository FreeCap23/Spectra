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
    Metal(dvec3 albedo, double roughness) :
        m_albedo(albedo), m_roughness(roughness) {}
    virtual bool scatter(
        Ray ray,
        hitRecord hits,
        dvec3& attenuation,
        Ray& scattered)
    const override {
        dvec3 reflected = Spectra::reflect(glm::normalize(ray.getDirection()), hits.normal);
        dvec3 roughReflected =
            reflected
            + m_roughness
            * Spectra::randomInUnitSphere();
        scattered = Ray(hits.intersection, roughReflected);
        attenuation = m_albedo;
        return glm::dot(reflected, hits.normal) > 0;
    }

 private:
    dvec3 m_albedo;
    double m_roughness;
};

class Dielectric : public Material {
 public:
    Dielectric(double ior) : m_ior(ior) {}
    virtual bool scatter (
        Ray ray,
        hitRecord hits,
        dvec3& attenuation,
        Ray& scattered)
    const override {
        attenuation = dvec3(1);
        double refractionRatio;
        if (hits.frontFace)
            refractionRatio = 1.0/m_ior;
        else
            refractionRatio = m_ior;

        dvec3 unitDir = glm::normalize(ray.getDirection());
        double cosTheta = fmin(glm::dot(-unitDir, hits.normal), 1.0);
        double sinTheta = sqrt(1 - cosTheta * cosTheta);
        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        dvec3 scatteredDir;
        if (cannotRefract || getReflectance(cosTheta, refractionRatio) > Spectra::randomDouble())
            scatteredDir = Spectra::reflect(unitDir, hits.normal);
        else
            scatteredDir = Spectra::refract(unitDir, hits.normal, refractionRatio);
        scattered = Ray(hits.intersection, scatteredDir);
        return true;
    }
 private:
    /**
     * Uses Schlick's polynomial approximation for reflectance
    */
    static double getReflectance(double cos, double refractionRatio) {
        double r0 = (1 - refractionRatio) / (1 + refractionRatio);
        r0 = r0 * r0; 
        return r0 + (1 - r0) * pow(1-cos, 5);
    }
 private:
    /**
     * Index of refraction
    */
    double m_ior;
};