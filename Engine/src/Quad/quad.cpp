#include "quad.hpp"

quad::quad(const point3 &Q, const vec3 &u, const vec3 &v,
           shared_ptr<material> mat)
    : Q(Q), u(u), v(v), mat(mat) {
    auto n = cross(u, v);
    normal = unit_vector(n);
    D = dot(normal, Q);
    w = n / dot(n, n);
    area = n.length();

    set_bounding_box();
}

void quad::set_bounding_box() {
    auto bbox_diagonal1 = aabb(Q, Q + u + v);
    auto bbox_diagonal2 = aabb(Q + u, Q + v);
    bbox = aabb(bbox_diagonal1, bbox_diagonal2);
}

aabb quad::bounding_box() const { return bbox; }

bool quad::hit(const ray &r, interval ray_t, hit_record &rec) const {
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

    if (!is_interior(alpha, beta, rec))
        return false;

    rec.t = t;
    rec.p = intersection;
    rec.mat_ptr = mat;
    rec.set_face_normal(r, normal);

    return true;
}

bool quad::is_interior(double a, double b, hit_record &rec) const {
    interval unit_interval = interval(0, 1);
    if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
        return false;
    }

    rec.u = a;
    rec.v = b;
    return true;
}

double quad::pdf_value(const point3 &origin, const vec3 &direction) const {
    hit_record rec;
    if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
        return 0;

    auto distance_squared = rec.t * rec.t * direction.length_squared();
    auto cosine = fabs(dot(direction, rec.normal) / direction.length());

    return distance_squared / (cosine * area);
}

vec3 quad::random(const point3 &origin) const {
    auto p = Q + (random_double() * u) + (random_double() * v);
    return p - origin;
}