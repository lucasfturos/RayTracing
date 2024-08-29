#include "camera.hpp"

void Camera::initialize() {
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
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors.
    auto defocus_radius = focus_dist * std::tan(radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;

    // Pre-calculate Perlin noise
    initializePerlinNoise();
}

void Camera::initializePerlinNoise() {
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
