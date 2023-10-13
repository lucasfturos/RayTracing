#include "render.hpp"

bvh_node Render::solar_scene() {
    const char *filename = "img/earthmap.jpg";
    auto solar_texture{make_shared<image_texture>(filename)};
    auto solar_surface{make_shared<lambertian>(solar_texture)};
    auto globe{make_shared<sphere>(point3(0, 0, 0), 2, solar_surface)};
    return bvh_node(hittable_list(globe), 0.0, 1.0);
}
