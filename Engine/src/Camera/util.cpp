#include "camera.hpp"

vec3 Camera::sampleSquare() const {
    // Retorna o vetor para um ponto aleatório no quadrado unitário
    // [-.5,-.5]-[+.5,+.5] usando ruído Perlin.
    int i = static_cast<int>(randomDouble() * image_width);
    int j = static_cast<int>(randomDouble() * image_height);
    vec3 noise = precomputed_noise[i][j];
    return vec3(noise.x - 0.5, noise.y - 0.5, 0);
}

vec3 Camera::sampleSquareStratified(int s_i, int s_j) const {
    int i = (s_i + static_cast<int>(randomDouble())) % image_width;
    int j = (s_j + static_cast<int>(randomDouble())) % image_height;
    vec3 noise = precomputed_noise[i][j];

    auto px = ((s_i + noise.x) * recip_sqrt_spp) - 0.5;
    auto py = ((s_j + noise.y) * recip_sqrt_spp) - 0.5;

    return vec3(px, py, 0);
}

point3 Camera::defocusDiskSample() const {
    // Returns a random point in the camera defocus disk.
    auto p = randomInUnitDisk();
    return center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
}

Ray Camera::getRay(int i, int j, int s_i, int s_j) const {
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
