#include "render.hpp"

bvh_node Render::single_scene() {
    hittable_list world;
    // Texture
    auto checker{make_shared<checker_texture>(color(0.0, 0.0, 0.0),
                                              color(0.9, 0.9, 0.9))};
    auto pertext{make_shared<noise_texture>(4)};

    // Material
    difflight = make_shared<diffuse_light>(color(4, 4, 4));
    material_metal = make_shared<metal>(color(.8, .8, .8), .0);
    material_lambertian = make_shared<lambertian>(color(0.255, 0.412, 0.882));
    material_lambertian_checker = make_shared<lambertian>(checker);
    material_lambertian_pertext = make_shared<lambertian>(pertext);
    material_dieletric = make_shared<dielectric>(1.5);

    // Chão
    box_ = make_shared<box>(point3(-2, -0.5, -2), point3(2, -0.6, 2),
                            material_lambertian);
    world.add(box_);
    // Objeto no centro
    world.add(make_shared<sphere>(point3(.0, .0, -1), .5, material_metal));
    // Objeto a esquerda
    // world.add(make_shared<sphere>(point3(-1.0, .0, -1.0), .5, difflight));
    // Objeto a direita
    // world.add(make_shared<sphere>(point3(1.0, .0, -1.0), .5, difflight));
    // Objeto em cima do centro
    // world.add(make_shared<sphere>(point3(.0, 1.2, -1), .5, difflight));

    return bvh_node(world, 0.0, 1.0);
}
