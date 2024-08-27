#pragma once

#include "../../include/threadpool.hpp"
#include "../../include/vec3.hpp"

#include <SDL2/SDL.h>
#include <iostream>

class Color {
  private:
    std::shared_ptr<ThreadPool> m_threadPool;

    int find_ansi_rgb(int r, int g, int b);
    int find_ansi_hsl(int h, int s, int l);
    int distance_hsl(int i, int h, int s, int l);
    double linear_to_gamma(double linear_component);

    void rgb_to_hsl(int r, int g, int b, int *h, int *s, int *l);

  public:
    Color();
    ~Color();

    void write_color(std::ostream &out, color pixel_color, double scale);
    void write_color_SDL(SDL_Surface *surface, color pixel_color, double scale,
                         int x, int y);

    void run_color(std::ostream &out, color pixel_color, double scale);
};
