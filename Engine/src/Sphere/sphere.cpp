#include "sphere.hpp"

bool Sphere::hit(const Ray &r, Interval ray_t, HitRecord &rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return false;
    }

    auto sqrtd = std::sqrt(discriminant);
    auto root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (-half_b + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }

    // Encontra a raiz mais próxima que esteja no intervalo aceitável
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.normal = outward_normal;
    rec.setFaceNormal(r, outward_normal);
    getSphereUV(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

double Sphere::pdfValue(const point3 &origin, const vec3 &direction) const {
    // This method only works for stationary spheres.
    HitRecord rec;
    if (!this->hit(Ray(origin, direction), Interval(0.001, infinity), rec))
        return 0;

    auto cos_theta_max =
        sqrt(1 - radius * radius / (center - origin).lengthSquared());
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

vec3 Sphere::random(const point3 &origin) const {
    vec3 direction = center - origin;
    auto distance_squared = direction.lengthSquared();
    ONB uvw;
    uvw.buildFromW(direction);
    return uvw.local(randomToSphere(radius, distance_squared));
}