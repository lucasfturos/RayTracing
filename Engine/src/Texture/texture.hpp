#pragma once

#include "math/perlin.hpp"
#include "utils/constante.hpp"

class Texture {
  public:
    virtual color value(double u, double v, const point3 &p) const = 0;
};

class SolidColor : public Texture {
  public:
    SolidColor() {}
    SolidColor(color c) : color_value(c) {}

    SolidColor(double red, double green, double blue)
        : SolidColor(color(red, green, blue)) {}

    virtual color value(double /* u */, double /* v */,
                        const vec3 & /* p */) const override {
        return color_value;
    }

  private:
    color color_value;
};

class CheckerTexture : public Texture {
  public:
    CheckerTexture() {}
    CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
        : even(_even), odd(_odd) {}

    CheckerTexture(color c1, color c2)
        : even(make_shared<SolidColor>(c1)), odd(make_shared<SolidColor>(c2)) {}

    virtual color value(double u, double v, const point3 &p) const override {
        auto sines{std::sin(10 * p.x) * std::sin(10 * p.y) *
                   std::sin(10 * p.z)};
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }

  public:
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class NoiseTexture : public Texture {
  public:
    NoiseTexture() {}
    NoiseTexture(double sc) : scale(sc) {}

    virtual color value(double /* u */, double /* v */,
                        const point3 &p) const override {
        // Textura de Perlin - Interpolação usando vetores aleatórios nos pontos
        // da grade
        // return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale *  p));

        // Textura de Perlin - Turbulência com ajuste de fase
        return color(1, 1, 1) * noise.turb(scale * p);

        // Textura de Perlin - Turbulência
        // return color(1, 1, 1) * noise.turb(p);

        // Textura de Perlin - Efeito de Marmore
        // return color(.5, .5, .5) *
        //        (1 + sin(scale * p.z + 10 * noise.turb(p, 7)));
    }

  public:
    Perlin noise;
    double scale;
};

class ImageTexture : public Texture {
  public:
    const static int bytes_per_pixel = 4;

    ImageTexture()
        : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    ImageTexture(const char *filename);

    ~ImageTexture();

    virtual color value(double u, double v, const vec3 &p) const override;

  private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
};
