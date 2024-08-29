#pragma once

#include "HitTable/hittable_list.hpp"
#include "math/onb.hpp"

class PDF {
  public:
    virtual ~PDF() {}

    virtual double value(const vec3 &direction) const = 0;
    virtual vec3 generate() const = 0;
};

class SpherePDF : public PDF {
  public:
    SpherePDF() {}

    double value(const vec3 & /* direction */) const override {
        return 1 / (4 * pi);
    }

    vec3 generate() const override { return randomUnitVector(); }
};

class CosinePDF : public PDF {
  public:
    CosinePDF(const vec3 &w) { uvw.buildFromW(w); }

    double value(const vec3 &direction) const override {
        auto cosine_theta = dot(unitVector(direction), uvw.w());
        return fmax(0, cosine_theta / pi);
    }

    vec3 generate() const override {
        return uvw.local(randomCosineDirection());
    }

  private:
    ONB uvw;
};

class HittablePDF : public PDF {
  public:
    HittablePDF(const HitTable &objects, const point3 &origin)
        : objects(objects), origin(origin) {}

    double value(const vec3 &direction) const override {
        return objects.pdfValue(origin, direction);
    }

    vec3 generate() const override { return objects.random(origin); }

  private:
    const HitTable &objects;
    point3 origin;
};

class MixturePDF : public PDF {
  public:
    MixturePDF(shared_ptr<PDF> p0, shared_ptr<PDF> p1) : p({p0, p1}) {}

    double value(const vec3 &direction) const override {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    vec3 generate() const override {
        if (randomDouble() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

  private:
    std::vector<shared_ptr<PDF>> p;
};
