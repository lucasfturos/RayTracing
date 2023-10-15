#include "cornell_box.hpp"

bvh_node CornellBox::cornell_smoke() {
    hittable_list objects;

    // Cores e luz
    red = make_shared<lambertian>(color(.65, .05, .05));
    white = make_shared<lambertian>(color(.73, .73, .73));
    green = make_shared<lambertian>(color(.12, .45, .15));
    light = make_shared<diffuse_light>(color(7, 7, 7));

    // Paredes e lampada
    objects.add(make_shared<yz_rect>(0, 277.5, 0, 277.5, 277.5, green));
    objects.add(make_shared<yz_rect>(0, 277.5, 0, 277.5, 0, red));
    objects.add(make_shared<xz_rect>(56.5, 221.5, 63.5, 216, 277, light));
    objects.add(make_shared<xz_rect>(0, 277.5, 0, 277.5, 277.5, white));
    objects.add(make_shared<xz_rect>(0, 277.5, 0, 277.5, 0, white));
    objects.add(make_shared<xy_rect>(0, 277.5, 0, 277.5, 277.5, white));

    // Caixas
    box1 = make_shared<box>(point3(0, 0, 0), point3(82.5, 165, 82.5), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(132.5, 0, 147.5));
    box2 = make_shared<box>(point3(0, 0, 0), point3(82.5, 82.5, 82.5), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(65, 0, 32.5));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return bvh_node(objects, 0.0, 1.0);
}

bvh_node CornellBox::cornell_box() {
    hittable_list objects;

    // Cores e luz
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto blue = make_shared<lambertian>(color(0., 0., 1.));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto metal_tex = make_shared<metal>(color(.8, .8, .8), .0);

    // Paredes e lâmpada
    objects.add(make_shared<yz_rect>(0, 138.75, 0, 138.75, 138.75, green));
    objects.add(make_shared<yz_rect>(0, 138.75, 0, 138.75, 0, red));
    objects.add(make_shared<xz_rect>(28.25, 110.75, 31.75, 108, 138.5, light));
    objects.add(make_shared<xz_rect>(0, 138.75, 0, 138.75, 0, white));
    objects.add(make_shared<xz_rect>(0, 138.75, 0, 138.75, 138.75, white));
    objects.add(make_shared<xy_rect>(0, 138.75, 0, 138.75, 138.75, white));

    box1 = make_shared<box>(point3(0, 0, 0), point3(82.5, 82.5, 82.5), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(132.5, 0, 147.5));
    objects.add(box1);

    box2 = make_shared<box>(point3(0, 0, 0), point3(82.5, 165, 82.5), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(65, 0, 32.5));
    objects.add(box2);

    std::vector<shared_ptr<hittable>> box_objects;
    box_objects.push_back(box1);
    box_objects.push_back(box2);

    // Crie uma hierarquia BVH com as caixas
    objects.add(
        make_shared<bvh_node>(box_objects, 0, box_objects.size(), 0, 1));

    return bvh_node(objects, 0.0, 1.0);
}
