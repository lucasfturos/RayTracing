#pragma once

#include "HitTable/hittable.hpp"
#include "Texture/texture.hpp"
#include "render/material.hpp"
#include "utils/constante.hpp"

class ConstantMedium : public HitTable {
  public:
    ConstantMedium(shared_ptr<HitTable> b, double d, shared_ptr<Texture> a)
        : boundary(b), neg_inv_density(-1 / d),
          phase_function(make_shared<Isotropic>(a)) {}

    ConstantMedium(shared_ptr<HitTable> b, double d, color c)
        : boundary(b), neg_inv_density(-1 / d),
          phase_function(make_shared<Isotropic>(c)) {}

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;

    AABB boundingBox() const override { return boundary->boundingBox(); }

  public:
    shared_ptr<HitTable> boundary;
    double neg_inv_density;
    shared_ptr<Material> phase_function;
};
