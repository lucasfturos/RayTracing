#pragma once

#include "../../include/constante.hpp"
#include "../HitTable/hittable_list.hpp"

class Quad : public HitTable {
  public:
    Quad(const point3 &Q, const vec3 &u, const vec3 &v,
         shared_ptr<Material> mat);

    virtual void setBoundingBox();

    virtual AABB boundingBox() const override;

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;

    virtual bool isInterior(double a, double b, HitRecord &rec) const;

    virtual double pdfValue(const point3 &origin,
                            const vec3 &direction) const override;

    virtual vec3 random(const point3 &origin) const override;

  private:
    point3 Q;
    vec3 u, v, w;
    vec3 normal;
    double D;
    double area;
    shared_ptr<Material> mat;
    AABB bbox;
};

inline shared_ptr<HittableList> box(const point3 &a, const point3 &b,
                                    shared_ptr<Material> mat) {
    // Returns the 3D box (six sides) that contains the two opposite vertices a
    // & b.

    auto sides = make_shared<HittableList>();

    // Construct the two opposite vertices with the minimum and maximum
    // coordinates.
    point3 min(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    point3 max(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));

    vec3 dx(max.x - min.x, 0, 0);
    vec3 dy(0, max.y - min.y, 0);
    vec3 dz(0, 0, max.z - min.z);

    sides->add(make_shared<Quad>(point3(min.x, min.y, max.z), dx, dy,
                                 mat)); // front
    sides->add(make_shared<Quad>(point3(max.x, min.y, max.z), -dz, dy,
                                 mat)); // right
    sides->add(make_shared<Quad>(point3(max.x, min.y, min.z), -dx, dy,
                                 mat)); // back
    sides->add(make_shared<Quad>(point3(min.x, min.y, min.z), dz, dy,
                                 mat)); // left
    sides->add(make_shared<Quad>(point3(min.x, max.y, max.z), dx, -dz,
                                 mat)); // top
    sides->add(make_shared<Quad>(point3(min.x, min.y, min.z), dx, dz,
                                 mat)); // bottom

    return sides;
}
