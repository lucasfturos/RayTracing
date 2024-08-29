#pragma once

#include "math/vec3.hpp"
#include "utils/threadpool.hpp"

#include <SDL2/SDL.h>
#include <iostream>

class Color {
  private:
    std::shared_ptr<ThreadPool> m_threadPool;

    int findAnsiRGB(int r, int g, int b);
    int findAnsiHSL(int h, int s, int l);
    int distanceHSL(int i, int h, int s, int l);
    double linearToGamma(double linear_component);

    void RGB2HSL(int r, int g, int b, int *h, int *s, int *l);

  public:
    Color();
    ~Color();

    void writeColor(std::ostream &out, color pixel_color, double scale);
    void writeColorSDL(SDL_Surface *surface, color pixel_color, double scale,
                       int x, int y);

    void runColor(std::ostream &out, color pixel_color, double scale);
};
