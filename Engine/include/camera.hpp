#pragma once

#include "../src/BVH/bvh.hpp"
#include "constante.hpp"
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
                    int depth) {
        hit_record rec;
        // Se exceder o limite do rebatimento dos pacotes de luz, não haverá
        // mais coleta de luz.
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        // Se o raio não atingir nada, retorna a cor de fundo.
        if (!root.hit(r, eps, infinity, rec)) {
            return background;
        }

        ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emitted;
        }
        return emitted +
               attenuation * ray_color(scattered, background, root, depth - 1);
    }

  private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
