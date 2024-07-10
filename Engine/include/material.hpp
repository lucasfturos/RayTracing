#pragma once

#include "../src/Texture/texture.hpp"
#include "pdf.hpp"

class scatter_record {
  public:
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
    bool skip_pdf;
    ray skip_pdf_ray;
};

class material {
  public:
    virtual color emitted(const ray & /* r_in */, const hit_record & /* rec */,
                          double /* u */, double /* v */,
                          const point3 & /* p */) const {
        return color(0, 0, 0);
    }

    virtual bool scatter(const ray & /* r_in */, const hit_record & /* rec */,
                         scatter_record & /* srec */) const {
        return false;
    }

    virtual double scattering_pdf(const ray & /* r_in */,
                                  const hit_record & /* rec */,
                                  const ray & /* scattered */) const {
        return 0.0;
    }
};

class lambertian : public material {
  public:
    lambertian(const color &a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(const ray & /* r_in */, const hit_record &rec,
                         scatter_record &srec) const override {
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray & /* r_in */, const hit_record &rec,
                          const ray &scattered) const override {
        auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
        return cos_theta < 0 ? 0 : cos_theta / pi;
    }

  public:
    shared_ptr<texture> albedo;
};

class metal : public material {
  public:
    metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         scatter_record &srec) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

        srec.attenuation = albedo;
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        srec.skip_pdf_ray = ray(rec.p, reflected, r_in.time());

        return true;
    }

  public:
    color albedo;
    double fuzz;
};

class dielectric : public material {
  public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         scatter_record &srec) const override {
        srec.attenuation = color(1.0, 1.0, 1.0);
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        double ri = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, ri);
        }

        srec.skip_pdf_ray = ray(rec.p, direction, r_in.time());
        return true;
    }

  public:
    double ir; // Índice de refração

  private:
    static double reflectance(double cosine, double ref_idx) {
        // Usa a aproximação de Schlick para refletância.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class diffuse_light : public material {
  public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    virtual color emitted(const ray & /* r_in */, const hit_record &rec,
                          double u, double v, const point3 &p) const override {
        if (!rec.front_face) {
            return color(0, 0, 0);
        }
        return emit->value(u, v, p);
    }

  public:
    shared_ptr<texture> emit;
};

class isotropic : public material {
  public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(const ray & /* r_in */, const hit_record &rec,
                         scatter_record &srec) const override {
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<sphere_pdf>();
        srec.skip_pdf = false;
        return true;
    }

    virtual double scattering_pdf(const ray & /* r_in */,
                                  const hit_record & /* rec */,
                                  const ray & /* scattered */) const override {
        return 1 / (4 * pi);
    }

  public:
    shared_ptr<texture> albedo;
};
