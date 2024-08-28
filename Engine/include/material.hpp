#pragma once

#include "../src/Texture/texture.hpp"
#include "pdf.hpp"
#include "ray.hpp"

class ScatterRecord {
  public:
    color attenuation;
    shared_ptr<PDF> pdf_ptr;
    bool skip_pdf;
    Ray skip_pdf_ray;
};

class Material {
  public:
    virtual color emitted(const Ray & /* r_in */, const HitRecord & /* rec */,
                          double /* u */, double /* v */,
                          const point3 & /* p */) const {
        return color(0, 0, 0);
    }

    virtual bool scatter(const Ray & /* r_in */, const HitRecord & /* rec */,
                         ScatterRecord & /* srec */) const {
        return false;
    }

    virtual double scatteringPDF(const Ray & /* r_in */,
                                 const HitRecord & /* rec */,
                                 const Ray & /* scattered */) const {
        return 0.0;
    }
};

class Lambertian : public Material {
  public:
    Lambertian(const color &a) : albedo(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(const Ray & /* r_in */, const HitRecord &rec,
                         ScatterRecord &srec) const override {
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<CosinePDF>(rec.normal);
        srec.skip_pdf = false;
        return true;
    }

    double scatteringPDF(const Ray & /* r_in */, const HitRecord &rec,
                         const Ray &scattered) const override {
        auto cos_theta = dot(rec.normal, unitVector(scattered.direction()));
        return cos_theta < 0 ? 0 : cos_theta / pi;
    }

  public:
    shared_ptr<Texture> albedo;
};

class Metal : public Material {
  public:
    Metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         ScatterRecord &srec) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unitVector(reflected) + (fuzz * randomUnitVector());

        srec.attenuation = albedo;
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        srec.skip_pdf_ray = Ray(rec.p, reflected, r_in.time());

        return true;
    }

  public:
    color albedo;
    double fuzz;
};

class Dielectric : public Material {
  public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         ScatterRecord &srec) const override {
        srec.attenuation = color(1.0, 1.0, 1.0);
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        double ri = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = unitVector(r_in.direction());
        double cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > randomDouble()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, ri);
        }

        srec.skip_pdf_ray = Ray(rec.p, direction, r_in.time());
        return true;
    }

  public:
    double ir; // Índice de refração

  private:
    static double reflectance(double cosine, double ref_idx) {
        // Usa a aproximação de Schlick para refletância.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class DiffuseLight : public Material {
  public:
    DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
    DiffuseLight(color c) : emit(make_shared<SolidColor>(c)) {}

    virtual color emitted(const Ray & /* r_in */, const HitRecord &rec,
                          double u, double v, const point3 &p) const override {
        if (!rec.front_face) {
            return color(0, 0, 0);
        }
        return emit->value(u, v, p);
    }

  public:
    shared_ptr<Texture> emit;
};

class Isotropic : public Material {
  public:
    Isotropic(color c) : albedo(make_shared<SolidColor>(c)) {}
    Isotropic(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(const Ray & /* r_in */, const HitRecord &rec,
                         ScatterRecord &srec) const override {
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<SpherePDF>();
        srec.skip_pdf = false;
        return true;
    }

    virtual double scatteringPDF(const Ray & /* r_in */,
                                 const HitRecord & /* rec */,
                                 const Ray & /* scattered */) const override {
        return 1 / (4 * pi);
    }

  public:
    shared_ptr<Texture> albedo;
};
