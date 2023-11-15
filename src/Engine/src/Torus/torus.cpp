#include "torus.hpp"

bool torus::hit(const ray &r, double t_min, double t_max,
                hit_record &rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - outter_radius * outter_radius -
             inner_radius * inner_radius;
    auto d = 2.0 * outter_radius;

    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return false;
    }

    auto sqrtd = sqrt(discriminant);
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }

    // Encontra a raiz mais próxima que esteja no intervalo aceitável
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center);
    vec3 normal_proj = outward_normal;
    normal_proj.e[1] = 0.0; // Projeta a normal para o plano Y=0
    outward_normal = unit_vector(normal_proj);
    rec.normal = outward_normal;
    rec.set_face_normal(r, outward_normal);

    // Corrige o cálculo das coordenadas UV
    get_torus_uv(rec.p - center, inner_radius, rec.u, rec.v);

    rec.mat_ptr = mat_ptr;

    return true;
}

bool torus::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(center - vec3(outter_radius + inner_radius,
                                    outter_radius + inner_radius,
                                    outter_radius + inner_radius),
                      center + vec3(outter_radius + inner_radius,
                                    outter_radius + inner_radius,
                                    outter_radius + inner_radius));
    return true;
}
