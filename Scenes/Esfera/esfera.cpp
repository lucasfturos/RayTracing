#include "esfera.hpp"

bvh_node Esfera::solar_scene() {
    const char *filename = "img/earthmap.jpg";
    auto solar_texture{make_shared<image_texture>(filename)};
    auto solar_surface{make_shared<lambertian>(solar_texture)};
    auto globe{make_shared<sphere>(point3(0, 0, 0), 2, solar_surface)};
    return bvh_node(hittable_list(globe), 0.0, 1.0);
}

bvh_node Esfera::single_scene() {
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

bvh_node Esfera::simple_light() {
    hittable_list objects;

    material_metal = make_shared<metal>(color(.8, .8, .8), .0);
    auto pertext{make_shared<noise_texture>(4)};
    box_ = make_shared<box>(point3(-2, -0.5, -2), point3(2, -0.6, 2),
                            material_metal);
    objects.add(box_);
    objects.add(make_shared<sphere>(point3(0, 0, 0), .5,
                                    make_shared<lambertian>(pertext)));

    difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(
        make_shared<box>(point3(-1, 0.7, -1), point3(1, -0.6, 1), difflight));

    return bvh_node(objects, 0.0, 1.0);
}

bvh_node Esfera::random_scene() {
    // World
    hittable_list world;

    material_lambertian = make_shared<lambertian>(color(.5, .5, .5));
    world.add(
        make_shared<sphere>(point3(0, -100, -1), 100, material_lambertian));

    for (int a = -2; a < 2; a++) {
        for (int b = -2; b < 2; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2,
                          b + 0.9 * random_double());

            if ((center - point3(4, .0, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    return bvh_node(world, 0.0, 1.0);
}
