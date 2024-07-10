#include "cornell_box.hpp"
#include "../../Engine/src/Constant_Medium/constant_medium.hpp"
#include "../../Engine/src/Quad/quad.hpp"
#include "../../Engine/src/Sphere/sphere.hpp"

Scene CornellBox::cornell_smoke() {
    hittable_list world;

    red = make_shared<lambertian>(color(.65, .05, .05));
    white = make_shared<lambertian>(color(.73, .73, .73));
    green = make_shared<lambertian>(color(.12, .45, .15));
    light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0),
                                vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0),
                                vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0),
                                vec3(0, 0, 305), light));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0),
                                vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0),
                                vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0),
                                vec3(0, 555, 0), white));

    shared_ptr<hittable> box1 =
        box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 =
        box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    hittable_list lights;
    auto m = shared_ptr<material>();
    lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                                 vec3(0, 0, -105), m));

    return {world, lights};
}

Scene CornellBox::cornell_box() {
    hittable_list world;

    // Cores e luz
    red = make_shared<lambertian>(color(.65, .05, .05));
    white = make_shared<lambertian>(color(.73, .73, .73));
    green = make_shared<lambertian>(color(.12, .45, .15));
    light = make_shared<diffuse_light>(color(15, 15, 15));

    // Cornell box sides
    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 0, 555),
                                vec3(0, 555, 0), green));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(0, 0, -555),
                                vec3(0, 555, 0), red));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0),
                                vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0),
                                vec3(0, 0, -555), white));
    world.add(make_shared<quad>(point3(555, 0, 555), vec3(-555, 0, 0),
                                vec3(0, 555, 0), white));

    // Light
    world.add(make_shared<quad>(point3(213, 554, 227), vec3(130, 0, 0),
                                vec3(0, 0, 105), light));

    // Box
    shared_ptr<hittable> box1 =
        box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    // Glass Sphere
    auto glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));

    // Light Sources
    hittable_list lights;
    auto m = shared_ptr<material>();
    lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                                 vec3(0, 0, -105), m));
    lights.add(make_shared<sphere>(point3(190, 90, 190), 90, m));

    return {world, lights};
}

Scene CornellBox::final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0),
                                vec3(0, 0, 265), light));

    auto center1 = point3(400, 400, 200);
    // auto center2 = center1 + vec3(30, 0, 0);
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50,
                                  make_shared<dielectric>(1.5)));
    world.add(
        make_shared<sphere>(point3(0, 150, 145), 50,
                            make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70,
                                        make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(
        make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000,
                                   make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(
        make_shared<image_texture>("assets/img/earthmap.jpg"));
    world.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220, 280, 300), 80,
                                  make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    world.add(make_shared<translate>(
        make_shared<rotate_y>(make_shared<bvh_node>(boxes2), 15),
        vec3(-100, 270, 395)));

    hittable_list lights;
    auto m = shared_ptr<material>();
    lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                                 vec3(0, 0, -105), m));

    return {world, lights};
}