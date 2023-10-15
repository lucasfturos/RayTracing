#include "bule.hpp"
#include "teapot.hpp"

bvh_node BuleUtah::single_scene() {
    hittable_list world;

    auto material_triangle = make_shared<lambertian>(color(.55, .62, .27));
    // auto material_triangle = make_shared<metal>(color(.7, .7, .7), 0);
    //  auto material_triangle = make_shared<dielectric>(0.2);
    auto material_ground = make_shared<lambertian>(color(.8, .8, .8));
    auto box_ = make_shared<box>(point3(-2, -0.5, -2), point3(2, -0.6, 2),
                                 material_ground);

    std::vector<shared_ptr<hittable>> triangles;
    for (int i = 0; i < teapot_indices.size() - 1; i += 3) {
        int index1 = teapot_indices[i + 0];
        int index2 = teapot_indices[i + 1];
        int index3 = teapot_indices[i + 2];

        auto triangle = make_shared<Triangle>(
            teapot_vertices[index1], teapot_vertices[index2],
            teapot_vertices[index3], material_triangle);
        triangles.push_back(triangle);
    }

    world.add(make_shared<bvh_node>(triangles, 0, triangles.size(), 0, 10));
    world.add(box_);

    return bvh_node(world, 0.0, 1.0);
}
