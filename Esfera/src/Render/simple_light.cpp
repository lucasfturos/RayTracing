#include "render.hpp"

bvh_node Render::simple_light() {
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
