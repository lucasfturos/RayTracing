#include "camera.hpp"

Camera::Camera(double aspect_ratio, int image_width, int samples_per_pixel,
               int max_depth, double vfov, double defocus_angle,
               double focus_dist, const point3 &lookfrom, const point3 &lookat,
               const vec3 &vup)
    : image_width(image_width), samples_per_pixel(samples_per_pixel),
      max_depth(max_depth), vfov(vfov), aspect_ratio(aspect_ratio),
      defocus_angle(defocus_angle), focus_dist(focus_dist), lookfrom(lookfrom),
      lookat(lookat), vup(vup), yaw(-90.0), pitch(0.0), zoom(1),
      perlin_noise(make_shared<Perlin>()) {
    initialize();
}

void Camera::render(
    const BVHNode &world, const HittableList &lights, const color &background,
    int j, std::function<void(int, double, const color &)> draw_pixel) {
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

void Camera::renderTile(
    int x0, int y0, int x1, int y1, const BVHNode &world,
    const HittableList &lights, const color &background,
    std::function<void(int, int, int, int, double, const color &)> draw_tile) {
    for (int j = y0; j < y1; ++j) {
        for (int i = x0; i < x1; ++i) {
            color pixel_color(0, 0, 0);
            for (int s_j = 0; s_j < sqrt_spp; ++s_j) {
                for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
                    std::pair<int, int> pixel_coord = {j, i};
                    if (pixel_cache.find(pixel_coord) != pixel_cache.end()) {
                        pixel_color += pixel_cache[pixel_coord];
                    } else {
                        auto r = getRay(i, j, s_i, s_j);
                        color sample_color =
                            rayColor(r, background, max_depth, world, lights);
                        pixel_cache[pixel_coord] = sample_color;
                        pixel_color += sample_color;
                    }
                }
            }
            draw_tile(i, j, i + 1, j + 1, pixel_samples_scale, pixel_color);
        }
    }
}

color Camera::rayColor(const Ray &r, color background, int depth,
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

void Camera::updateCameraVectors() {
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