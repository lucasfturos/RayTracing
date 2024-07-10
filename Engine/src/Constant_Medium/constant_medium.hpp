#pragma once

#include "../../include/constante.hpp"
#include "../../include/material.hpp"
#include "../HitTable/hittable.hpp"
#include "../Texture/texture.hpp"

class constant_medium : public hittable {
  public:
    constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
        : boundary(b), neg_inv_density(-1 / d),
          phase_function(make_shared<isotropic>(a)) {}

    constant_medium(shared_ptr<hittable> b, double d, color c)
        : boundary(b), neg_inv_density(-1 / d),
          phase_function(make_shared<isotropic>(c)) {}

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    aabb bounding_box() const override { return boundary->bounding_box(); }

  public:
    shared_ptr<hittable> boundary;
    double neg_inv_density;
    shared_ptr<material> phase_function;
};
