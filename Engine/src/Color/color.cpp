#include "color.hpp"

static const interval intensity(0.0, 0.999);

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

    float cmax = std::max({r_, g_, b_});
    float cmin = std::min({r_, g_, b_});
    float delta = cmax - cmin;

    // Hue calculation
    float hf = 0.0f;
    if (delta < 1e-6f) {
        hf = 0.0f;
    } else {
        if (cmax == r_) {
            hf = 60.0f * fmodf((g_ - b_) / delta, 6.0f);
        } else if (cmax == g_) {
            hf = 60.0f * ((b_ - r_) / delta + 2.0f);
        } else if (cmax == b_) {
            hf = 60.0f * ((r_ - g_) / delta + 4.0f);
        }
    }

    // Lightness calculation
    float lf = (cmax + cmin) / 2.0f;

    // Saturation calculation
    float sf = 0.0f;
    if (delta < 1e-6f) {
        sf = 0.0f;
    } else {
        sf = delta / (1.0f - std::abs(2.0f * lf - 1.0f));
    }

    *h = static_cast<int>(fmodf(hf, 360.0f) + 360.0f) % 360;
    *s = static_cast<int>(sf * 100.0f);
    *l = static_cast<int>(lf * 100.0f);
}

double Color::linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return sqrt(linear_component);
    }
    return 0;
}

void Color::run_color(std::ostream &out, color pixel_color, double scale) {
    auto r = pixel_color.x * scale;
    auto g = pixel_color.y * scale;
    auto b = pixel_color.z * scale;

    int h, s, l;
    rgb_to_hsl(static_cast<int>(256 * intensity.clamp(r)),
               static_cast<int>(256 * intensity.clamp(g)),
               static_cast<int>(256 * intensity.clamp(b)), &h, &s, &l);

    int ansi_index = find_ansi_hsl(h, s, l);
    auto chosen_color = table_rgb[ansi_index];
    out << "\033[48;5;" << chosen_color << "m  ";
}

void Color::write_color(std::ostream &out, color pixel_color, double scale) {
    color color = pixel_color;

    color.x != color.x ? color.x = 0.0 : 0;
    color.y != color.y ? color.y = 0.0 : 0;
    color.z != color.z ? color.z = 0.0 : 0;

    // Divide a cor pelo número de amostras.
    color.x *= scale;
    color.y *= scale;
    color.z *= scale;

    // color.x = linear_to_gamma(color.x);
    // color.y = linear_to_gamma(color.y);
    // color.z = linear_to_gamma(color.z);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255 * intensity.clamp(color.x)) << ' '
        << static_cast<int>(255 * intensity.clamp(color.y)) << ' '
        << static_cast<int>(255 * intensity.clamp(color.z)) << '\n';
}

void Color::write_color_SDL(SDL_Renderer *renderer, color pixel_color,
                            double scale) {
    color color = pixel_color;

    color.x != color.x ? color.x = 0.0 : 0;
    color.y != color.y ? color.y = 0.0 : 0;
    color.z != color.z ? color.z = 0.0 : 0;

    color.x *= scale;
    color.y *= scale;
    color.z *= scale;

    color.x = linear_to_gamma(color.x);
    color.y = linear_to_gamma(color.y);
    color.z = linear_to_gamma(color.z);

    SDL_Color sdl_color = {static_cast<Uint8>(256 * intensity.clamp(color.x)),
                           static_cast<Uint8>(256 * intensity.clamp(color.y)),
                           static_cast<Uint8>(256 * intensity.clamp(color.z)),
                           SDL_ALPHA_OPAQUE};

    SDL_SetRenderDrawColor(renderer, sdl_color.r, sdl_color.g, sdl_color.b,
                           sdl_color.a);
}
