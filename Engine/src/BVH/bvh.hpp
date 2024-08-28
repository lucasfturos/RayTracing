#pragma once

#include "../HitTable/hittable_list.hpp"

class BVHNode : public HitTable {
  public:
    BVHNode() = default;

    BVHNode(HittableList list)
        : BVHNode(list.objects, 0, list.objects.size()) {}

    BVHNode(const std::vector<shared_ptr<HitTable>> objects, size_t start,
            size_t end);

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;
    AABB boundingBox() const override { return bbox; }

    static bool boxCompare(const shared_ptr<HitTable> a,
                           const shared_ptr<HitTable> b, int axis);
    static bool boxCompareX(const shared_ptr<HitTable> &a,
                            const shared_ptr<HitTable> &b);
    static bool boxCompareY(const shared_ptr<HitTable> &a,
                            const shared_ptr<HitTable> &b);
    static bool boxCompareZ(const shared_ptr<HitTable> &a,
                            const shared_ptr<HitTable> &b);

  public:
    shared_ptr<HitTable> left;
    shared_ptr<HitTable> right;
    AABB bbox;
};
