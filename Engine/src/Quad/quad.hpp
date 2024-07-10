#pragma once

#include "../../include/constante.hpp"
#include "../HitTable/hittable_list.hpp"

class quad : public hittable {
  public:
    quad(const point3 &Q, const vec3 &u, const vec3 &v,
         shared_ptr<material> mat);

    virtual void set_bounding_box();

    virtual aabb bounding_box() const override;

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    virtual bool is_interior(double a, double b, hit_record &rec) const;

  private:
    point3 Q;
    vec3 u, v, w;
    vec3 normal;
    double D;
    shared_ptr<material> mat;
    aabb bbox;
};

inline shared_ptr<hittable_list> box(const point3 &a, const point3 &b,
                                     shared_ptr<material> mat) {
    // Returns the 3D box (six sides) that contains the two opposite vertices a
    // & b.

    auto sides = make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum
    // coordinates.
    auto min =
        point3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    auto max =
        point3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));

    auto dx = vec3(max.x - min.x, 0, 0);
    auto dy = vec3(0, max.y - min.y, 0);
    auto dz = vec3(0, 0, max.z - min.z);

    sides->add(make_shared<quad>(point3(min.x, min.y, max.z), dx, dy,
                                 mat)); // front
    sides->add(make_shared<quad>(point3(max.x, min.y, max.z), -dz, dy,
                                 mat)); // right
    sides->add(make_shared<quad>(point3(max.x, min.y, min.z), -dx, dy,
                                 mat)); // back
    sides->add(make_shared<quad>(point3(min.x, min.y, min.z), dz, dy,
                                 mat)); // left
    sides->add(make_shared<quad>(point3(min.x, max.y, max.z), dx, -dz,
                                 mat)); // top
    sides->add(make_shared<quad>(point3(min.x, min.y, min.z), dx, dz,
                                 mat)); // bottom

    return sides;
}
