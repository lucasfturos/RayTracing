#pragma once

#include "../../include/constante.hpp"
#include "../../include/onb.hpp"
#include "../HitTable/hittable.hpp"

class Sphere : public HitTable {
  public:
    Sphere() {}
    Sphere(point3 cen, double r, shared_ptr<Material> m)
        : center(cen), radius(r), mat_ptr(m) {
        auto rvec = vec3(radius, radius, radius);
        bbox = AABB(center - rvec, center + rvec);
    };

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;
    virtual AABB boundingBox() const override { return bbox; }
    double pdfValue(const point3 &origin, const vec3 &direction) const override;
    vec3 random(const point3 &origin) const override;

  private:
    static void getSphereUV(const point3 &p, double &u, double &v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta{std::acos(-p.y)};
        auto phi{std::atan2(-p.z, p.x) + pi};

        u = phi / (2 * pi);
        v = theta / pi;
    }

    static vec3 randomToSphere(double radius, double distance_squared) {
        auto r1 = randomDouble();
        auto r2 = randomDouble();
        auto z = 1 + r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1);

        auto phi = 2 * pi * r1;
        auto x = std::cos(phi) * std::sqrt(1 - z * z);
        auto y = std::sin(phi) * std::sqrt(1 - z * z);

        return vec3(x, y, z);
    }

  public:
    point3 center;
    double radius;
    shared_ptr<Material> mat_ptr;
    AABB bbox;
};
