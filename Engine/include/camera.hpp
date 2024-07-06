#pragma once

#include "../src/BVH/bvh.hpp"
#include "constante.hpp"
#include "interval.hpp"
#include "material.hpp"

class camera {
  public:
    camera(point3 lookfrom, point3 lookat, vec3 vup,
           double vfov, // campo de visão vertical em graus
           double aspect_ratio) {
        auto theta{degrees_to_radians(vfov)};
        auto h{std::tan(theta / 2)};
        auto viewport_height{2.0 * h};
        auto viewport_width{aspect_ratio * viewport_height};

        auto w{unit_vector(lookfrom - lookat)};
        auto u{unit_vector(cross(vup, w))};
        auto v{cross(w, u)};

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    ray get_ray(double s, double t) const {
        return ray(origin,
                   lower_left_corner + s * horizontal + t * vertical - origin);
    }

    color ray_color(const ray &r, const color &background, const bvh_node &root,
                    int max_depth) {
        color current_attenuation = color(1.0, 1.0, 1.0);
        ray current_ray = r;

        for (int depth = 0; depth < max_depth; ++depth) {
            hit_record rec;

            if (!root.hit(current_ray, interval(eps, infinity), rec)) {
                return current_attenuation * background;
            }

            color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
            ray scattered;
            color attenuation;

            if (rec.mat_ptr->scatter(current_ray, rec, attenuation,
                                     scattered)) {
                current_attenuation *= attenuation;
                current_ray = scattered;
            } else {
                return current_attenuation * emitted;
            }
        }

        // Se atingir a profundidade máxima, retorna preto
        return color(0, 0, 0);
    }

  private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
