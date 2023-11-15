#pragma once

#include "../../include/constante.hpp"
#include "../HitTable/hittable.hpp"

class torus : public hittable {
  public:
    torus() {}
    torus(point3 cen, double r_outter, double r_inner, shared_ptr<material> m)
        : center(cen), outter_radius(r_outter), inner_radius(r_inner),
          mat_ptr(m){};

    virtual bool hit(const ray &r, double t_min, double t_max,
                     hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1,
                              aabb &output_box) const override;

  private:
    static void get_torus_uv(const point3 &p, double inner_r, double &u,
                             double &v) {
        auto phi = atan2(p.z(), p.x());
        if (phi < 0.0) {
            phi += 2.0 * pi;
        }

        auto theta = asin(p.y() / inner_r);

        // Mapeia phi para u e theta para v
        u = phi / (2.0 * pi);
        v = (theta + pi / 2.0) / pi;
    }

  public:
    point3 center;
    double outter_radius;
    double inner_radius;
    shared_ptr<material> mat_ptr;
};
