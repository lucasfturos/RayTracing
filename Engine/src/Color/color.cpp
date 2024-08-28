#include "color.hpp"
#include "../Interval/interval.hpp"
#include "table_color.hpp"

static const Interval intensity(0.0, 0.999);

Color::Color() : m_threadPool(std::make_shared<ThreadPool>()) {
    m_threadPool->start();
}

Color::~Color() { m_threadPool->stop(); }

int Color::distanceHSL(int i, int h, int s, int l) {
    int dh{h - table_hsl[i][0]};
    int ds{s - table_hsl[i][1]};
    int dl{l - table_hsl[i][2]};
    return dh * dh + ds * ds + dl * dl;
}

int Color::findAnsiHSL(int h, int s, int l) {
    int index{0};
    int min_distance{std::numeric_limits<int>::max()};

    for (int i{}; i < 256; ++i) {
        int distance{distanceHSL(i, h, s, l)};
        if (distance < min_distance) {
            min_distance = distance;
            index = i;
        }
    }
    return index;
}

void Color::RGB2HSL(int r, int g, int b, int *h, int *s, int *l) {
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
            hf = 60.0f * std::fmod((g_ - b_) / delta, 6.0f);
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

    *h = static_cast<int>(std::fmod(hf, 360.0f) + 360.0f) % 360;
    *s = static_cast<int>(sf * 100.0f);
    *l = static_cast<int>(lf * 100.0f);
}

double Color::linearToGamma(double linear_component) {
    if (linear_component > 0) {
        return sqrt(linear_component);
    }
    return 0;
}

void Color::runColor(std::ostream &out, color pixel_color, double scale) {
    auto r = pixel_color.x * scale;
    auto g = pixel_color.y * scale;
    auto b = pixel_color.z * scale;

    int h, s, l;
    RGB2HSL(static_cast<int>(256 * intensity.clamp(r)),
            static_cast<int>(256 * intensity.clamp(g)),
            static_cast<int>(256 * intensity.clamp(b)), &h, &s, &l);

    int ansi_index = findAnsiHSL(h, s, l);
    auto chosen_color = table_rgb[ansi_index];
    out << "\033[48;5;" << chosen_color << "m  ";
}

void Color::writeColor(std::ostream &out, color pixel_color, double scale) {
    color color = pixel_color * scale;
    color.x != color.x ? color.x = 0.0 : 0;
    color.y != color.y ? color.y = 0.0 : 0;
    color.z != color.z ? color.z = 0.0 : 0;

    color.x = linearToGamma(color.x);
    color.y = linearToGamma(color.y);
    color.z = linearToGamma(color.z);

    out << static_cast<int>(255 * intensity.clamp(color.x)) << ' '
        << static_cast<int>(255 * intensity.clamp(color.y)) << ' '
        << static_cast<int>(255 * intensity.clamp(color.z)) << '\n';
}

void Color::writeColorSDL(SDL_Surface *surface, color pixel_color, double scale,
                          int x, int y) {
    m_threadPool->enqueue([this, surface, pixel_color, scale, x, y] {
        color scaled_color = pixel_color * scale;
        scaled_color.x != scaled_color.x ? scaled_color.x = 0.0 : 0;
        scaled_color.y != scaled_color.y ? scaled_color.y = 0.0 : 0;
        scaled_color.z != scaled_color.z ? scaled_color.z = 0.0 : 0;

        scaled_color.x = linearToGamma(scaled_color.x);
        scaled_color.y = linearToGamma(scaled_color.y);
        scaled_color.z = linearToGamma(scaled_color.z);

        Uint8 r = static_cast<Uint8>(256 * intensity.clamp(scaled_color.x));
        Uint8 g = static_cast<Uint8>(256 * intensity.clamp(scaled_color.y));
        Uint8 b = static_cast<Uint8>(256 * intensity.clamp(scaled_color.z));
        Uint32 color = (255 << 24) | (r << 16) | (g << 8) | b;

        Uint32 *pixels = (Uint32 *)surface->pixels;
        pixels[y * surface->w + x] = color;
    });
}
