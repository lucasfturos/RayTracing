#include "cornell_box.hpp"
#include "../../Engine/src/Constant_Medium/constant_medium.hpp"
#include "../../Engine/src/Quad/quad.hpp"
#include "../../Engine/src/Sphere/sphere.hpp"

Scene CornellBox::cornell_smoke() {
    HittableList world;

    red = make_shared<Lambertian>(color(.65, .05, .05));
    white = make_shared<Lambertian>(color(.73, .73, .73));
    green = make_shared<Lambertian>(color(.12, .45, .15));
    light = make_shared<DiffuseLight>(color(7, 7, 7));

    world.add(make_shared<Quad>(point3(555, 0, 0), vec3(0, 555, 0),
                                vec3(0, 0, 555), green));
    world.add(make_shared<Quad>(point3(0, 0, 0), vec3(0, 555, 0),
                                vec3(0, 0, 555), red));
    world.add(make_shared<Quad>(point3(113, 554, 127), vec3(330, 0, 0),
                                vec3(0, 0, 305), light));
    world.add(make_shared<Quad>(point3(0, 555, 0), vec3(555, 0, 0),
                                vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(point3(0, 0, 0), vec3(555, 0, 0),
                                vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(point3(0, 0, 555), vec3(555, 0, 0),
                                vec3(0, 555, 0), white));

    shared_ptr<HitTable> box1 =
        box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, vec3(265, 0, 295));

    shared_ptr<HitTable> box2 =
        box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, vec3(130, 0, 65));

    world.add(make_shared<ConstantMedium>(box1, 0.01, color(0, 0, 0)));
    world.add(make_shared<ConstantMedium>(box2, 0.01, color(1, 1, 1)));

    HittableList lights;
    auto m = shared_ptr<Material>();
    lights.add(make_shared<Quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                                 vec3(0, 0, -105), m));

    return {world, lights};
}

Scene CornellBox::cornell_box() {
    HittableList world;

    // Cores e luz
    red = make_shared<Lambertian>(color(.65, .05, .05));
    white = make_shared<Lambertian>(color(.73, .73, .73));
    green = make_shared<Lambertian>(color(.12, .45, .15));
    light = make_shared<DiffuseLight>(color(15, 15, 15));

    // Cornell box sides
    world.add(make_shared<Quad>(point3(555, 0, 0), vec3(0, 0, 555),
                                vec3(0, 555, 0), green));
    world.add(make_shared<Quad>(point3(0, 0, 555), vec3(0, 0, -555),
                                vec3(0, 555, 0), red));
    world.add(make_shared<Quad>(point3(0, 555, 0), vec3(555, 0, 0),
                                vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(point3(0, 0, 555), vec3(555, 0, 0),
                                vec3(0, 0, -555), white));
    world.add(make_shared<Quad>(point3(555, 0, 555), vec3(-555, 0, 0),
                                vec3(0, 555, 0), white));

    // Light
    world.add(make_shared<Quad>(point3(213, 554, 227), vec3(130, 0, 0),
                                vec3(0, 0, 105), light));

    // Box
    shared_ptr<Material> aluminum =
        make_shared<Metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<HitTable> box1 =
        box(point3(0, 0, 0), point3(165, 330, 165), aluminum);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    shared_ptr<HitTable> box2 =
        box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, vec3(130, 0, 65));
    world.add(box2);

    // Light Sources
    HittableList lights;
    auto m = shared_ptr<Material>();
    lights.add(make_shared<Quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                                 vec3(0, 0, -105), m));

    return {world, lights};
}

Scene CornellBox::final_scene() {
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    HittableList world;

    world.add(make_shared<BVHNode>(boxes1));

    auto light = make_shared<DiffuseLight>(color(7, 7, 7));
    world.add(make_shared<Quad>(point3(123, 554, 147), vec3(300, 0, 0),
                                vec3(0, 0, 265), light));

    auto center1 = point3(400, 400, 200);
    // auto center2 = center1 + vec3(30, 0, 0);
    auto sphere_material = make_shared<Lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<Sphere>(center1, 50, sphere_material));

    world.add(make_shared<Sphere>(point3(260, 150, 45), 50,
                                  make_shared<Dielectric>(1.5)));
    world.add(
        make_shared<Sphere>(point3(0, 150, 145), 50,
                            make_shared<Metal>(color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<Sphere>(point3(360, 150, 145), 70,
                                        make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(point3(0, 0, 0), 5000,
                                   make_shared<Dielectric>(1.5));
    world.add(make_shared<ConstantMedium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<Lambertian>(
        make_shared<ImageTexture>("assets/img/earthmap.jpg"));
    world.add(make_shared<Sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.2);
    world.add(make_shared<Sphere>(point3(220, 280, 300), 80,
                                  make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = make_shared<Lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(point3::random(0, 165), 10, white));
    }

    world.add(make_shared<Translate>(
        make_shared<RotateY>(make_shared<BVHNode>(boxes2), 15),
        vec3(-100, 270, 395)));

    HittableList lights;
    auto m = shared_ptr<Material>();
    lights.add(make_shared<Quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                                 vec3(0, 0, -105), m));

    return {world, lights};
}