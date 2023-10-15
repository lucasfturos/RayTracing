#pragma once

#include "../../Engine/include/camera.hpp"
#include "../../Engine/include/perlin.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Box/box.hpp"
#include "../../Engine/src/Color/color.hpp"
#include "../../Engine/src/Constant_Medium/constant_medium.hpp"
#include "../../Engine/src/HitTable/hittable_list.hpp"
#include <SDL2/SDL.h>

class Render {
  private:
    // Window
    const int screen_width{1080};
    const int screen_height{720};
    const char *title{"Ray Tracing - Render"};

    // SDL2
    SDL_Window *win;
    SDL_Renderer *ren;
    void initSDL2();

    // Imagem
    const double aspect_ratio{3.0 / 2.0}; // Proporção 3:2
    // const double aspect_ratio{1.0}; // Proporção 1:1
    // const double aspect_ratio{16.0 / 9.0}; // Proporção 16:9
    const int image_width{
        500}; // Limite da função do run_term é 200 image_width
    const int image_height{static_cast<int>(image_width / aspect_ratio)};
    const int samples_per_pixel{100};
    const int max_depth{50};

    // Camera
    shared_ptr<camera> cam;
    const double aperture{2.0};
    const double vfov{50.0};

    // Color
    shared_ptr<Color> color_ptr;
    color ray_color(const ray &r, const color &background, const bvh_node &root,
                    int depth);

    // Objeto
    bvh_node world;

  public:
    Render(const bvh_node &root, int opc);
    ~Render();
    void run();
    void run_ppm();
    void run_term();
};
