#pragma once

#include "../src/BVH/bvh.hpp"
#include "../src/Interval/interval.hpp"
#include "constante.hpp"
#include "material.hpp"
#include <functional>

class camera {
  public:
    camera(double aspect_ratio, int image_width, int samples_per_pixel,
           int max_depth, double vfov, double defocus_angle, double focus_dist,
           const point3 &lookfrom, const point3 &lookat, const vec3 &vup)
        : image_width(image_width), samples_per_pixel(samples_per_pixel),
          max_depth(max_depth), vfov(vfov), aspect_ratio(aspect_ratio),
          defocus_angle(defocus_angle), focus_dist(focus_dist),
          lookfrom(lookfrom), lookat(lookat), vup(vup) {
        initialize();
    }

    void render(const bvh_node &world, const color &background, int j,
                std::function<void(int, const color &)> draw_pixel) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                ray r = get_ray(i, j);
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
    double defocus_angle;
    double focus_dist;

    point3 lookfrom;
    point3 lookat;
    vec3 vup;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u; // Defocus disk horizontal radius
    vec3 defocus_disk_v; // Defocus disk vertical radius

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        // Determine viewport dimensions.
        // auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width =
            viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate
        // frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical
        // viewport edges.
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to
        // pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc =
            viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius =
            focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at
        // randomly sampled point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x) * pixel_delta_u) +
                            ((j + offset.y) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
        // square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
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
