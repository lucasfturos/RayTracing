#pragma once

#include "AABB/aabb.hpp"

class Material;

struct HitRecord {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    shared_ptr<Material> mat_ptr;
    double u, v;

    inline void setFaceNormal(const Ray &r, const vec3 &outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class HitTable {
  public:
    virtual bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const = 0;

    virtual AABB boundingBox() const = 0;

    virtual double pdfValue(const point3 & /* origin */,
                            const vec3 & /* direction */) const {
        return 0.0;
    }

    virtual vec3 random(const point3 & /* origin */) const {
        return vec3(1, 0, 0);
    }
};

class Translate : public HitTable {
  public:
    Translate(shared_ptr<HitTable> object, const vec3 &offset)
        : object(object), offset(offset) {
        bbox = object->boundingBox() + offset;
    }

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;

    AABB boundingBox() const override { return bbox; }

  public:
    shared_ptr<HitTable> object;
    vec3 offset;
    AABB bbox;
};

class RotateY : public HitTable {
  public:
    RotateY(shared_ptr<HitTable> p, double angle);

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;

    virtual AABB boundingBox() const override { return bbox; }

  public:
    shared_ptr<HitTable> ptr;
    double sin_theta;
    double cos_theta;
    AABB bbox;
};
