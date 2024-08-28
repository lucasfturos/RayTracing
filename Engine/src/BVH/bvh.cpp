#include "bvh.hpp"

bool BVHNode::boxCompare(const shared_ptr<HitTable> a,
                         const shared_ptr<HitTable> b, int axis) {
    auto a_axis_interval = a->boundingBox().axisInterval(axis);
    auto b_axis_interval = b->boundingBox().axisInterval(axis);
    return a_axis_interval.min < b_axis_interval.min;
}

bool BVHNode::boxCompareX(const shared_ptr<HitTable> &a,
                          const shared_ptr<HitTable> &b) {
    return boxCompare(a, b, 0);
}

bool BVHNode::boxCompareY(const shared_ptr<HitTable> &a,
                          const shared_ptr<HitTable> &b) {
    return boxCompare(a, b, 1);
}

bool BVHNode::boxCompareZ(const shared_ptr<HitTable> &a,
                          const shared_ptr<HitTable> &b) {
    return boxCompare(a, b, 2);
}

BVHNode::BVHNode(std::vector<shared_ptr<HitTable>> objects, size_t start,
                 size_t end) {
    bbox = AABB::empty;
    for (size_t object_index = start; object_index < end; ++object_index) {
        bbox = AABB(bbox, objects[object_index]->boundingBox());
    }

    int axis = bbox.longestAxis();
    auto comparator = (axis == 0)   ? boxCompareX
                      : (axis == 1) ? boxCompareY
                                    : boxCompareZ;

    size_t object_span = end - start;
    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<BVHNode>(objects, start, mid);
        right = make_shared<BVHNode>(objects, mid, end);
    }
    bbox = AABB(left->boundingBox(), right->boundingBox());
}

bool BVHNode::hit(const Ray &r, Interval ray_t, HitRecord &rec) const {
    if (!bbox.hit(r, ray_t)) {
        return false;
    }

    bool hit_left = left->hit(r, ray_t, rec);
    bool hit_right =
        right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

    return hit_left || hit_right;
}
