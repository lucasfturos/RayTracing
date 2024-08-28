#pragma once

#include "../src/BVH/bvh.hpp"
#include "constante.hpp"
#include "material.hpp"
#include "pdf.hpp"
#include "perlin.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <functional>

struct PixelHash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ h2;
    }
};

class Camera {
  public:
    Camera(double aspect_ratio, int image_width, int samples_per_pixel,
           int max_depth, double vfov, double defocus_angle, double focus_dist,
           const point3 &lookfrom, const point3 &lookat, const vec3 &vup)
        : image_width(image_width), samples_per_pixel(samples_per_pixel),
          max_depth(max_depth), vfov(vfov), aspect_ratio(aspect_ratio),
          defocus_angle(defocus_angle), focus_dist(focus_dist),
          lookfrom(lookfrom), lookat(lookat), vup(vup), yaw(-90.0), pitch(0.0),
          zoom(1), perlin_noise(make_shared<Perlin>()) {
        initialize();
    }

    void render(const BVHNode &world, const HittableList &lights,
                const color &background, int j,
                std::function<void(int, double, const color &)> draw_pixel) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s_j = 0; s_j < sqrt_spp; ++s_j) {
                for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
                    std::pair<int, int> pixel_coord = {j, i};
                    if (pixel_cache.find(pixel_coord) != pixel_cache.end()) {
                        pixel_color += pixel_cache[pixel_coord];
                    } else {
                        Ray r = getRay(i, j, s_i, s_j);
                        pixel_color +=
                            rayColor(r, background, max_depth, world, lights);
                        pixel_cache[pixel_coord] = pixel_color;
                    }
                }
            }
            draw_pixel(i, pixel_samples_scale, pixel_color);
        }
    }

    void
    renderTile(int x0, int y0, int x1, int y1, const BVHNode &world,
               const HittableList &lights, const color &background,
               std::function<void(int, int, int, int, double, const color &)>
                   draw_tile) {
        for (int j = y0; j < y1; ++j) {
            for (int i = x0; i < x1; ++i) {
                color pixel_color(0, 0, 0);
                for (int s_j = 0; s_j < sqrt_spp; ++s_j) {
                    for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
                        std::pair<int, int> pixel_coord = {j, i};
                        if (pixel_cache.find(pixel_coord) !=
                            pixel_cache.end()) {
                            pixel_color += pixel_cache[pixel_coord];
                        } else {
                            auto r = getRay(i, j, s_i, s_j);
                            color sample_color = rayColor(
                                r, background, max_depth, world, lights);
                            pixel_cache[pixel_coord] = sample_color;
                            pixel_color += sample_color;
                        }
                    }
                }
                draw_tile(i, j, i + 1, j + 1, pixel_samples_scale, pixel_color);
            }
        }
    }

    void processMouseMovement(int deltaX, int deltaY) {
        constexpr double sensitivity = 0.1;
        Interval interval(-89.0, 89.0);
        yaw += deltaX * sensitivity;
        pitch += deltaY * sensitivity;
        pitch = interval.clamp(pitch);
        updateCameraVectors();
        pixel_cache.clear();
    }

    void processMouseScroll(int deltaY) {
        constexpr double zoom_factor = 0.1;
        Interval interval(-45.0, 45.0);
        zoom += deltaY * zoom_factor;
        zoom = interval.clamp(zoom);
        updateCameraVectors();
        pixel_cache.clear();
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

    shared_ptr<Perlin> perlin_noise;

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
    std::unordered_map<std::pair<int, int>, vec3, PixelHash> pixel_cache;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        sqrt_spp = static_cast<int>(std::sqrt(samples_per_pixel));
        pixel_samples_scale = 1.0 / (sqrt_spp * sqrt_spp);
        recip_sqrt_spp = 1.0 / sqrt_spp;

        // Determine viewport dimensions.
        auto theta = radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width =
            viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate
        // frame.
        w = unitVector(lookfrom - lookat);
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical
        // viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

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
        auto defocus_radius = focus_dist * std::tan(radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;

        // Pre-calculate Perlin noise
        initializePerlinNoise();
    }

    Ray getRay(int i, int j, int s_i, int s_j) const {
        // Construct a camera ray originating from the defocus disk and
        // directed at a randomly sampled point around the pixel location i,
        // j for stratified sample square s_i, s_j.
        auto offset = sampleSquareStratified(s_i, s_j);
        auto pixel_sample = pixel00_loc + ((i + offset.x) * pixel_delta_u) +
                            ((j + offset.y) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocusDiskSample();
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    void initializePerlinNoise() {
        precomputed_noise.resize(image_width);
        for (int i = 0; i < image_width; ++i) {
            precomputed_noise[i].resize(image_height);
            for (int j = 0; j < image_height; ++j) {
                precomputed_noise[i][j] =
                    vec3(perlin_noise->noise(vec3(i / 100.0, j / 100.0, 0)),
                         perlin_noise->noise(vec3(i / 100.0, j / 100.0, 1)), 0);
            }
        }
    }

    vec3 sampleSquare() const {
        // Retorna o vetor para um ponto aleatório no quadrado unitário
        // [-.5,-.5]-[+.5,+.5] usando ruído Perlin.
        int i = static_cast<int>(randomDouble() * image_width);
        int j = static_cast<int>(randomDouble() * image_height);
        vec3 noise = precomputed_noise[i][j];
        return vec3(noise.x - 0.5, noise.y - 0.5, 0);
    }

    vec3 sampleSquareStratified(int s_i, int s_j) const {
        int i = (s_i + static_cast<int>(randomDouble())) % image_width;
        int j = (s_j + static_cast<int>(randomDouble())) % image_height;
        vec3 noise = precomputed_noise[i][j];

        auto px = ((s_i + noise.x) * recip_sqrt_spp) - 0.5;
        auto py = ((s_j + noise.y) * recip_sqrt_spp) - 0.5;

        return vec3(px, py, 0);
    }

    point3 defocusDiskSample() const {
        // Returns a random point in the camera defocus disk.
        auto p = randomInUnitDisk();
        return center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
    }

    void updateCameraVectors() {
        vec3 front;
        front.x = std::cos(radians(yaw)) * std::cos(radians(pitch));
        front.y = std::sin(radians(pitch));
        front.z = std::sin(radians(yaw)) * std::cos(radians(pitch));
        front = unitVector(front);

        w = unitVector(lookfrom - (lookfrom + front));
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        auto theta = radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2.0 * h * focus_dist / -zoom;
        auto viewport_width = aspect_ratio * viewport_height;

        auto horizontal = viewport_width * u;
        auto vertical = viewport_height * v;
        pixel00_loc = lookfrom - horizontal / 2 - vertical / 2 - focus_dist * w;
        pixel_delta_u = horizontal / image_width;
        pixel_delta_v = vertical / image_height;
    }

    color rayColor(const Ray &r, color background, int depth,
                   const HitTable &world, const HitTable &lights) const {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        HitRecord rec;

        if (!world.hit(r, Interval(0.001, infinity), rec)) {
            return background;
        }

        ScatterRecord srec;
        color color_from_emission =
            rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

        if (!rec.mat_ptr->scatter(r, rec, srec)) {
            return color_from_emission;
        }

        if (srec.skip_pdf) {
            return srec.attenuation * rayColor(srec.skip_pdf_ray, background,
                                               depth - 1, world, lights);
        }

        auto light_ptr = make_shared<HittablePDF>(lights, rec.p);
        MixturePDF p(light_ptr, srec.pdf_ptr);

        Ray scattered(rec.p, p.generate(), r.time());
        auto pdf_val = p.value(scattered.direction());

        double scattering_pdf = rec.mat_ptr->scatteringPDF(r, rec, scattered);

        color sample_color =
            rayColor(scattered, background, depth - 1, world, lights);
        color color_from_scatter =
            (srec.attenuation * scattering_pdf * sample_color) / pdf_val;

        return color_from_emission + color_from_scatter;
    }
};
