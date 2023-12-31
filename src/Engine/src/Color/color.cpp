#include "color.hpp"

int Color::distance_hsl(int i, int h, int s, int l) {
    int dh{h - table_hsl[i][0]};
    int ds{s - table_hsl[i][1]};
    int dl{l - table_hsl[i][2]};
    return dh * dh + ds * ds + dl * dl;
}

int Color::find_ansi_hsl(int h, int s, int l) {
    int index{0};
    int min_distance{std::numeric_limits<int>::max()};

    for (int i{}; i < 256; ++i) {
        int distance{distance_hsl(i, h, s, l)};
        if (distance < min_distance) {
            min_distance = distance;
            index = i;
        }
    }
    return index;
}

void Color::rgb_to_hsl(int r, int g, int b, int *h, int *s, int *l) {
    float r_ = r / 255.0f;
    float g_ = g / 255.0f;
    float b_ = b / 255.0f;

    float epsilon{1e-6};
    float cmax{r_};

    if (g_ > cmax) {
        cmax = g_;
    }
    if (b_ > cmax) {
        cmax = b_;
    }

    float cmin{r_};
    if (g_ < cmin) {
        cmin = g_;
    }
    if (b_ < cmin) {
        cmin = b_;
    }

    float delta{cmax - cmin};

    // H
    float hf{0};
    if (delta < epsilon) {
        hf = 0;
    } else if (cmax == r_) {
        hf = 60 * fmodf((g_ - b_) / delta, 6.0f);
    } else if (cmax == g_) {
        hf = 60 * ((b_ - r_) / delta + 2);
    } else if (cmax == b_) {
        hf = 60 * ((r_ - g_) / delta + 4);
    } else {
        assert(0 && "Inacessivel");
    }

    float lf{(cmax + cmin) / 2.0f}; // L

    // S
    float sf{0};
    if (delta < epsilon) {
        sf = 0.0;
    } else {
        sf = delta / (1 - fabsf(2 * lf - 1));
    }

    *h = fmodf(fmodf(hf, 360.0f) + 360.0f, 360.0f);
    *s = (sf * 100.0f);
    *l = (lf * 100.0f);
}

void Color::run_color(std::ostream &out, color pixel_color,
                      int samples_per_pixel) {
    auto scale = 1.0 / samples_per_pixel;
    auto r = pixel_color.x() * scale;
    auto g = pixel_color.y() * scale;
    auto b = pixel_color.z() * scale;

    int h, s, l;
    rgb_to_hsl(static_cast<int>(255 * clamp(r, 0.0, 0.999)),
               static_cast<int>(255 * clamp(g, 0.0, 0.999)),
               static_cast<int>(255 * clamp(b, 0.0, 0.999)), &h, &s, &l);

    int ansi_index = find_ansi_hsl(h, s, l);
    auto chosen_color = table_rgb[ansi_index];
    out << "\033[48;5;" << ansi_index << "m  ";
}

void Color::write_color(std::ostream &out, color pixel_color,
                        int samples_per_pixel) {
    color color = pixel_color;

    color.x() != color.x() ? color.e[0] = 0.0 : 0;
    color.y() != color.y() ? color.e[1] = 0.0 : 0;
    color.z() != color.z() ? color.e[2] = 0.0 : 0;

    // Divide a cor pelo número de amostras.
    auto scale = 1.0 / samples_per_pixel;
    color.e[0] *= scale;
    color.e[1] *= scale;
    color.e[2] *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255 * clamp(color.x(), 0.0, 0.999)) << ' '
        << static_cast<int>(255 * clamp(color.y(), 0.0, 0.999)) << ' '
        << static_cast<int>(255 * clamp(color.z(), 0.0, 0.999)) << '\n';
}

void Color::write_color_SDL(SDL_Renderer *renderer, color pixel_color,
                            int samples_per_pixel) {
    color color = pixel_color;

    color.x() != color.x() ? color.e[0] = 0.0 : 0;
    color.y() != color.y() ? color.e[1] = 0.0 : 0;
    color.z() != color.z() ? color.e[2] = 0.0 : 0;

    auto scale = 1.0 / samples_per_pixel;
    color.e[0] *= scale;
    color.e[1] *= scale;
    color.e[2] *= scale;

    SDL_Color sdl_color = {
        static_cast<Uint8>(255 * clamp(color.x(), 0.0, 0.999)),
        static_cast<Uint8>(255 * clamp(color.y(), 0.0, 0.999)),
        static_cast<Uint8>(255 * clamp(color.z(), 0.0, 0.999)), 255};

    SDL_SetRenderDrawColor(renderer, sdl_color.r, sdl_color.g, sdl_color.b,
                           sdl_color.a);
}
