#pragma once

#include "../src/BVH/bvh.hpp"
#include "constante.hpp"
#include "material.hpp"
#include "perlin.hpp"
#include <functional>

class camera {
  public:
    camera(double aspect_ratio, int image_width, int samples_per_pixel,
           int max_depth, double vfov, double defocus_angle, double focus_dist,
           const point3 &lookfrom, const point3 &lookat, const vec3 &vup)
        : image_width(image_width), samples_per_pixel(samples_per_pixel),
          max_depth(max_depth), vfov(vfov), aspect_ratio(aspect_ratio),
          defocus_angle(defocus_angle), focus_dist(focus_dist),
          lookfrom(lookfrom), lookat(lookat), vup(vup), yaw(-90.0), pitch(0.0),
          zoom(1), perlin_noise() {
        initialize();
    }

    void render(const bvh_node &world, const hittable_list &lights,
                const color &background, int j,
                std::function<void(int, double, const color &)> draw_pixel) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s_j = 0; s_j < sqrt_spp; s_j++) {
                for (int s_i = 0; s_i < sqrt_spp; s_i++) {
                    ray r = get_ray(i, j, s_i, s_j);
                    pixel_color +=
                        ray_color(r, background, max_depth, world, lights);
                }
            }
            draw_pixel(i, pixel_samples_scale, pixel_color);
        }
    }

    void processMouseMovement(int deltaX, int deltaY) {
        double sensitivity = 0.1;
        yaw += deltaX * sensitivity;
        pitch += deltaY * sensitivity;
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        updateCameraVectors();
    }

    void processMouseScroll(int deltaY) {
        double zoom_factor = 0.1;
        zoom += deltaY * zoom_factor;
        if (zoom < -45.0f) {
            zoom = -45.0f;
        }
        if (zoom > 45.0f) {
            zoom = 45.0f;
        }
        updateCameraVectors();
    }

    int getHeight() { return image_height; }

  private:
    // Variáveis do Construtor
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

    double yaw;
    double pitch;
    double zoom;

    perlin perlin_noise;

    // Variáveis privadas
    point3 center;
    point3 pixel00_loc;

    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;

    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    int sqrt_spp;
    double recip_sqrt_spp;
    double pixel_samples_scale;

    std::vector<std::vector<vec3>> precomputed_noise;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        sqrt_spp = static_cast<int>(std::sqrt(samples_per_pixel));
        pixel_samples_scale = 1.0 / (sqrt_spp * sqrt_spp);
        recip_sqrt_spp = 1.0 / sqrt_spp;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
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

        // Pre-calculate Perlin noise
        initialize_perlin_noise();
    }

    ray get_ray(int i, int j, int s_i, int s_j) const {
        // Construct a camera ray originating from the defocus disk and directed
        // at a randomly sampled point around the pixel location i, j for
        // stratified sample square s_i, s_j.
        auto offset = sample_square_stratified(s_i, s_j);
        auto pixel_sample = pixel00_loc + ((i + offset.x) * pixel_delta_u) +
                            ((j + offset.y) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    void initialize_perlin_noise() {
        precomputed_noise.resize(image_width);
        for (int i = 0; i < image_width; ++i) {
            precomputed_noise[i].resize(image_height);
            for (int j = 0; j < image_height; ++j) {
                precomputed_noise[i][j] = vec3(
                    perlin_noise.noise(vec3(random_double(), random_double(),
                                            random_double())),
                    perlin_noise.noise(vec3(random_double(), random_double(),
                                            random_double())),
                    0);
            }
        }
    }

    vec3 sample_square() const {
        // Retorna o vetor para um ponto aleatório no quadrado unitário
        // [-.5,-.5]-[+.5,+.5] usando ruído Perlin.
        int i = static_cast<int>(random_double() * image_width);
        int j = static_cast<int>(random_double() * image_height);
        vec3 noise = precomputed_noise[i][j];
        return vec3(noise.x - 0.5, noise.y - 0.5, 0);
    }

    vec3 sample_square_stratified(int s_i, int s_j) const {
        int i = (s_i + static_cast<int>(random_double())) % image_width;
        int j = (s_j + static_cast<int>(random_double())) % image_height;
        vec3 noise = precomputed_noise[i][j];

        auto px = ((s_i + noise.x) * recip_sqrt_spp) - 0.5;
        auto py = ((s_j + noise.y) * recip_sqrt_spp) - 0.5;

        return vec3(px, py, 0);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
    }

    void updateCameraVectors() {
        vec3 front;
        front.x = std::cos(degrees_to_radians(yaw)) *
                  std::cos(degrees_to_radians(pitch));
        front.y = std::sin(degrees_to_radians(pitch));
        front.z = std::sin(degrees_to_radians(yaw)) *
                  std::cos(degrees_to_radians(pitch));
        front = unit_vector(front);

        w = unit_vector(lookfrom - (lookfrom + front));
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2.0 * h * focus_dist / -zoom;
        auto viewport_width = aspect_ratio * viewport_height;

        vec3 horizontal = viewport_width * u;
        vec3 vertical = viewport_height * v;
        pixel00_loc = lookfrom - horizontal / 2 - vertical / 2 - focus_dist * w;
        pixel_delta_u = horizontal / image_width;
        pixel_delta_v = vertical / image_height;
    }

    color ray_color(const ray &r, color background, int depth,
                    const hittable &world, const hittable &lights) const {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hit_record rec;

        if (!world.hit(r, interval(0.001, infinity), rec)) {
            return background;
        }

        scatter_record srec;
        color color_from_emission =
            rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

        if (!rec.mat_ptr->scatter(r, rec, srec)) {
            return color_from_emission;
        }

        if (srec.skip_pdf) {
            return srec.attenuation * ray_color(srec.skip_pdf_ray, background,
                                                depth - 1, world, lights);
        }

        auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
        mixture_pdf p(light_ptr, srec.pdf_ptr);

        ray scattered = ray(rec.p, p.generate(), r.time());
        auto pdf_val = p.value(scattered.direction());

        double scattering_pdf = rec.mat_ptr->scattering_pdf(r, rec, scattered);

        color sample_color =
            ray_color(scattered, background, depth - 1, world, lights);
        color color_from_scatter =
            (srec.attenuation * scattering_pdf * sample_color) / pdf_val;

        return color_from_emission + color_from_scatter;
    }
};
