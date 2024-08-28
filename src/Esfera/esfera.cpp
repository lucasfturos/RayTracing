#include "esfera.hpp"
#include "../../Engine/src/Quad/quad.hpp"
#include "../../Engine/src/Sphere/sphere.hpp"

Scene Esfera::solar_scene() {
    const char *filename = "assets/img/earthmap.jpg";
    auto solar_texture{make_shared<ImageTexture>(filename)};
    auto solar_surface{make_shared<Lambertian>(solar_texture)};
    auto globe{make_shared<Sphere>(point3(0, 0, 0), 2, solar_surface)};
    auto m = shared_ptr<Material>();
    lights.add(make_shared<Sphere>(point3(0, 0, 0), 2, m));
    return {HittableList(globe), lights};
}

Scene Esfera::single_scene() {
    // Texture
    auto checker{make_shared<CheckerTexture>(color(0.0, 0.0, 0.0),
                                             color(0.9, 0.9, 0.9))};
    auto pertext{make_shared<NoiseTexture>(4)};

    // Material
    difflight = make_shared<DiffuseLight>(color(4, 4, 4));
    material_metal = make_shared<Metal>(color(.8, .8, .8), .0);
    material_lambertian = make_shared<Lambertian>(color(0.255, 0.412, 0.882));
    material_lambertian_checker = make_shared<Lambertian>(checker);
    material_lambertian_pertext = make_shared<Lambertian>(pertext);
    material_dieletric = make_shared<Dielectric>(1.5);

    // Chão
    world.add(make_shared<Quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0),
                                difflight));
    //  Objeto no centro
    world.add(make_shared<Sphere>(point3(0, -0.5, 0), 0.5, material_metal));
    // Objeto a esquerda
    // world.add(make_shared<sphere>(point3(-1.0, .0, -1.0), .5, difflight));
    // Objeto a direita
    // world.add(make_shared<sphere>(point3(1.0, .0, -1.0), .5, difflight));
    // Objeto em cima do centro
    // world.add(make_shared<sphere>(point3(.0, 1.2, -1), .5, difflight));

    auto m = shared_ptr<Material>();
    lights.add(
        make_shared<Quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), m));

    return {world, lights};
}

Scene Esfera::simple_light() {
    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000,
                                  make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(point3(0, 2, 0), 2,
                                  make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(color(4, 4, 4));
    world.add(make_shared<Sphere>(point3(0, 7, 0), 2, difflight));

    world.add(make_shared<Quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0),
                                difflight));

    auto m = shared_ptr<Material>();
    lights.add(make_shared<Sphere>(point3(0, 7, 0), 2, m));
    lights.add(
        make_shared<Quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), m));

    return {world, lights};
}

Scene Esfera::random_scene() {
    material_lambertian = make_shared<Lambertian>(color(.5, .5, .5));
    world.add(
        make_shared<Sphere>(point3(0, -100, -1), 100, material_lambertian));

    for (int a = -2; a < 2; a++) {
        for (int b = -2; b < 2; b++) {
            auto choose_mat = randomDouble();
            point3 center(a + 0.9 * randomDouble(), 0.2,
                          b + 0.9 * randomDouble());

            if ((center - point3(4, .0, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(
                        make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(
                        make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    return {world, lights};
}
