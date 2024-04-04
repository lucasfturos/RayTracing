#include "obj.hpp"
// #include "../../Engine/src/Box/box.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Triangle/triangle.hpp"
#include <glm/fwd.hpp>

Object::Object(const std::string &filepath) {
    read_object_ptr = make_shared<ReadObjectFile>(filepath);
}

bvh_node Object::single_scene() {
    hittable_list world;

    auto material_triangle = make_shared<lambertian>(color(.55, .62, .27));
    // auto material_triangle = make_shared<metal>(color(.7, .7, .7), 0);
    //  auto material_triangle = make_shared<dielectric>(0.2);
    auto material_ground = make_shared<lambertian>(color(.8, .8, .8));
    // auto box_ = make_shared<box>(point3(-2, -0.5, -2), point3(2, -0.6, 2),
    //                            material_ground);

    std::vector<shared_ptr<hittable>> triangles;
    std::vector<glm::vec3> vertices = read_object_ptr->sources().vertices;
    std::vector<glm::ivec3> faces = read_object_ptr->sources().faces;

    for (std::size_t i = 0; i < faces.size(); ++i) {
        vec3 v1 = vec3(vertices[faces[i].x].x, vertices[faces[i].x].y,
                       vertices[faces[i].x].z);
        vec3 v2 = vec3(vertices[faces[i].y].x, vertices[faces[i].y].y,
                       vertices[faces[i].y].z);
        vec3 v3 = vec3(vertices[faces[i].z].x, vertices[faces[i].z].y,
                       vertices[faces[i].z].z);

        auto triangle = make_shared<Triangle>(v1, v2, v3, material_triangle);
        triangles.push_back(triangle);
    }

    world.add(make_shared<bvh_node>(triangles, 0, triangles.size(), 0, 1));
    // world.add(box_);

    return bvh_node(world, 0.0, 1.0);
}
