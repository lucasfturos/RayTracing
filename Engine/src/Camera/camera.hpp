#pragma once

#include "BVH/bvh.hpp"
#include "math/perlin.hpp"
#include "render/material.hpp"
#include "render/pdf.hpp"
#include "utils/constante.hpp"
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
           const point3 &lookfrom, const point3 &lookat, const vec3 &vup);

    // Camera
    void render(const BVHNode &world, const HittableList &lights,
                const color &background, int j,
                std::function<void(int, double, const color &)> draw_pixel);
    void
    renderTile(int x0, int y0, int x1, int y1, const BVHNode &world,
               const HittableList &lights, const color &background,
               std::function<void(int, int, int, int, double, const color &)>
                   draw_tile);

    // Event
    void processMouseScroll(int deltaY);
    void processMouseMovement(int deltaX, int deltaY);

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

  private:
    // Initialize
    void initialize();
    void initializePerlinNoise();

    // Util
    vec3 sampleSquare() const;
    point3 defocusDiskSample() const;
    Ray getRay(int i, int j, int s_i, int s_j) const;
    vec3 sampleSquareStratified(int s_i, int s_j) const;

    // Camera
    void updateCameraVectors();
    color rayColor(const Ray &r, color background, int depth,
                   const HitTable &world, const HitTable &lights) const;
};
