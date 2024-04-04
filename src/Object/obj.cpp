#include "obj.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Box/box.hpp"
#include "../../Engine/src/Triangle/triangle.hpp"

Object::Object(const std::string &filepath) {
    read_object_ptr = make_shared<ReadObjectFile>(filepath);
}

bvh_node Object::single_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(0.0, 0.0, 0.0),
                                                color(0.9, 0.9, 0.9));

    // auto material_triangle = make_shared<lambertian>(color(.55, .62, .27));
    // auto material_triangle = make_shared<lambertian>(checker);
    auto material_triangle = make_shared<metal>(color(.55, .62, .27), 0);
    // auto material_triangle = make_shared<dielectric>(2.5);

    // auto material_ground = make_shared<lambertian>(color(.8, .8, .8));
    auto material_ground = make_shared<lambertian>(checker);
    auto box_ = make_shared<box>(point3(-2, -0.5, -2), point3(2, -0.6, 2),
                                 material_ground);

    std::vector<shared_ptr<hittable>> triangles;
    std::vector<vec3> vertices = read_object_ptr->sources().vertices;
    std::vector<ivec3> faces = read_object_ptr->sources().faces;

    for (std::size_t i = 0; i < faces.size(); ++i) {
        int index0 = faces[i].x;
        int index1 = faces[i].y;
        int index2 = faces[i].z;
        vec3 v0 = vertices[index0];
        vec3 v1 = vertices[index1];
        vec3 v2 = vertices[index2];

        auto triangle = make_shared<Triangle>(v0, v1, v2, material_triangle);
        triangles.push_back(triangle);
    }

    world.add(make_shared<bvh_node>(triangles, 0, triangles.size(), 0, 1));
    world.add(box_);

    return bvh_node(world, 0.0, 1.0);
}
