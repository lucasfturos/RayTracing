#include "render.hpp"

hittable_list Render::single_scene() {
    hittable_list world;

    point3 v0(0, 0, 0);
    point3 v1(1, 0, 0);
    point3 v2(0.5, 1, 0);
    vec3 normal0(0, 0, 1);
    vec3 normal1(0, 0, 1);
    vec3 normal2(0, 0, 1);

    auto material_triangle1 = make_shared<lambertian>(color(.73, .73, .73));

    auto triangle1 = make_shared<NormalTriangle>(v0, v1, v2, normal0, normal1,
                                                 normal2, material_triangle1);


    world.add(triangle1);

    return world;
}
