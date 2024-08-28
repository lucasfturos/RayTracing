#include "quad.hpp"

Quad::Quad(const point3 &Q, const vec3 &u, const vec3 &v,
           shared_ptr<Material> mat)
    : Q(Q), u(u), v(v), mat(mat) {
    auto n = cross(u, v);
    normal = unitVector(n);
    D = dot(normal, Q);
    w = n / dot(n, n);
    area = n.length();

    setBoundingBox();
}

void Quad::setBoundingBox() {
    AABB bbox_diagonal1(Q, Q + u + v);
    AABB bbox_diagonal2(Q + u, Q + v);
    bbox = AABB(bbox_diagonal1, bbox_diagonal2);
}

AABB Quad::boundingBox() const { return bbox; }

bool Quad::hit(const Ray &r, Interval ray_t, HitRecord &rec) const {
    auto denom = dot(normal, r.direction());

    if (std::abs(denom) < eps)
        return false;

    auto t = (D - dot(normal, r.origin())) / denom;
    if (!ray_t.contains(t))
        return false;

    auto intersection = r.at(t);
    vec3 planar_hitpt_vector = intersection - Q;
    auto alpha = dot(w, cross(planar_hitpt_vector, v));
    auto beta = dot(w, cross(u, planar_hitpt_vector));

    if (!isInterior(alpha, beta, rec))
        return false;

    rec.t = t;
    rec.p = intersection;
    rec.mat_ptr = mat;
    rec.setFaceNormal(r, normal);

    return true;
}

bool Quad::isInterior(double a, double b, HitRecord &rec) const {
    Interval unit_interval(0, 1);
    if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
        return false;
    }

    rec.u = a;
    rec.v = b;
    return true;
}

double Quad::pdfValue(const point3 &origin, const vec3 &direction) const {
    HitRecord rec;
    if (!this->hit(Ray(origin, direction), Interval(0.001, infinity), rec))
        return 0;

    auto distance_squared = rec.t * rec.t * direction.lengthSquared();
    auto cosine = std::abs(dot(direction, rec.normal) / direction.length());

    return distance_squared / (cosine * area);
}

vec3 Quad::random(const point3 &origin) const {
    auto p = Q + (randomDouble() * u) + (randomDouble() * v);
    return p - origin;
}