#include "render.hpp"

color Render::ray_color(const ray &r, const color &background,
                        const bvh_node &root, int depth) {
    hit_record rec;
    // Se exceder o limite do rebatimento dos pacotes de luz, não haverá mais
    // coleta de luz.
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // Se o raio não atingir nada, retorna a cor de fundo.
    if (!root.hit(r, .0001, infinity, rec)) {
        return background;
    }

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }
    return emitted +
           attenuation * ray_color(scattered, background, root, depth - 1);
}
