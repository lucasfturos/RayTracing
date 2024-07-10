#pragma once

#include "../AABB/aabb.hpp"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    shared_ptr<material> mat_ptr;
    double u, v;

    inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;

    virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
  public:
    translate(shared_ptr<hittable> p, const vec3 &displacement)
        : ptr(p), offset(displacement) {}

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    aabb bounding_box() const override { return bbox; }

  public:
    shared_ptr<hittable> ptr;
    vec3 offset;
    aabb bbox;
};

class rotate_y : public hittable {
  public:
    rotate_y(shared_ptr<hittable> p, double angle);

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    virtual aabb bounding_box() const override { return bbox; }

  public:
    shared_ptr<hittable> ptr;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};
