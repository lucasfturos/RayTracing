#pragma once

#include "../../Engine/include/camera.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Color/color.hpp"
#include <SDL2/SDL.h>

class Render {
  private:
    // Window
    const int screen_width{1080};
    const int screen_height{720};
    std::string title{"Ray Tracing - Window Render"};

    // SDL2
    SDL_Window *win;
    SDL_Renderer *ren;
    void initSDL2();

    // Imagem
    int image_width; // Limite da função do run_term é 200 image_width
    int image_height;
    const int samples_per_pixel{10};

    // Camera
    shared_ptr<camera> cam;

    // Color
    shared_ptr<Color> color_ptr;

    // Objeto
    bvh_node world;

  public:
    Render(const bvh_node &root, int opc);
    ~Render();
    void run();
    void run_ppm();
    void run_term();
};
