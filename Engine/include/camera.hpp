#pragma once

#include "../src/BVH/bvh.hpp"
#include "constante.hpp"
#include "interval.hpp"
#include "material.hpp"
#include <functional>

class camera {
  public:
    camera(double aspect_ratio, int image_width, int samples_per_pixel,
           int max_depth, double vfov, const point3 &lookfrom,
           const point3 &lookat, const vec3 &vup)
        : image_width(image_width), samples_per_pixel(samples_per_pixel),
          max_depth(max_depth), vfov(vfov), aspect_ratio(aspect_ratio),
          lookfrom(lookfrom), lookat(lookat), vup(vup) {
        initialize();
    }

    void render(const bvh_node &world, const color &background, int j,
                std::function<void(int, const color &)> draw_pixel) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                ray r = get_ray(u, v);
                pixel_color += ray_color(r, background, world);
            }
            draw_pixel(i, pixel_color);
        }
    }

    int getHeight() { return image_height; }

  private:
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;

    double vfov;
    double aspect_ratio;

    point3 lookfrom;
    point3 lookat;
    vec3 vup;
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

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

    color ray_color(const ray &r, const color &background,
                    const bvh_node &root) {
        color current_attenuation = color(1.0, 1.0, 1.0);
        ray current_ray = r;
        hit_record rec;
        ray scattered;
        color attenuation, emitted;

        for (int depth = 0; depth < max_depth; ++depth) {
            if (!root.hit(current_ray, interval(0.001, infinity), rec)) {
                return current_attenuation * background;
            }

            emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

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
};
