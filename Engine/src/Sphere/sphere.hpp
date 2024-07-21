#pragma once

#include "../../include/constante.hpp"
#include "../../include/onb.hpp"
#include "../HitTable/hittable.hpp"

class sphere : public hittable {
  public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(m) {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center - rvec, center + rvec);
    };

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    virtual aabb bounding_box() const override { return bbox; }

    double pdf_value(const point3 &origin,
                     const vec3 &direction) const override {
        // This method only works for stationary spheres.

        hit_record rec;
        if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
            return 0;

        auto cos_theta_max =
            sqrt(1 - radius * radius / (center - origin).length_squared());
        auto solid_angle = 2 * pi * (1 - cos_theta_max);

        return 1 / solid_angle;
    }

    vec3 random(const point3 &origin) const override {
        vec3 direction = center - origin;
        auto distance_squared = direction.length_squared();
        onb uvw;
        uvw.build_from_w(direction);
        return uvw.local(random_to_sphere(radius, distance_squared));
    }

  private:
    static void get_sphere_uv(const point3 &p, double &u, double &v) {
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

    static vec3 random_to_sphere(double radius, double distance_squared) {
        auto r1 = random_double();
        auto r2 = random_double();
        auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

        auto phi = 2 * pi * r1;
        auto x = std::cos(phi) * std::sqrt(1 - z * z);
        auto y = std::sin(phi) * std::sqrt(1 - z * z);

        return vec3(x, y, z);
    }

  public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
    aabb bbox;
};
