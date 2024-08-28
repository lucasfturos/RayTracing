#include "obj.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Quad/quad.hpp"
#include "../../Engine/src/Triangle/triangle.hpp"

Object::Object(const std::string &filepath) {
    read_object_ptr = make_shared<ReadObjectFile>(filepath);
}

Scene Object::single_scene() {
    auto pertext = make_shared<NoiseTexture>(4);
    auto checker =
        make_shared<CheckerTexture>(color(0.0, 0.0, 0.0), color(0.9, 0.9, 0.9));
    auto difflight = make_shared<DiffuseLight>(color(4, 4, 4));
    // auto material_triangle = make_shared<Lambertian>(color(.55, .62, .27));
    // auto material_triangle = make_shared<Lambertian>(pertext);
    // auto material_triangle = make_shared<Lambertian>(checker);
    auto material_triangle = make_shared<Metal>(color(.55, .62, .27), 0);
    // auto material_triangle = make_shared<Dielectric>(1.5);
    // auto material_triangle = make_shared<Lambertian>(
    // make_shared<ImageTexture>("assets/img/earthmap.jpg"));

    // auto material_ground = make_shared<lambertian>(color(.5, .0, .8));
    // auto material_ground = make_shared<lambertian>(checker);

    float scale = 1.0;
    std::vector<shared_ptr<HitTable>> triangles;
    std::vector<vec3> vertices = read_object_ptr->sources().vertices;
    std::vector<ivec3> faces = read_object_ptr->sources().faces;
    std::vector<vec2> textures = read_object_ptr->sources().textures;

    for (std::size_t i = 0; i < faces.size(); ++i) {
        int index0 = faces[i].x;
        int index1 = faces[i].y;
        int index2 = faces[i].z;
        vec3 v0 = vertices[index0] * scale;
        vec3 v1 = vertices[index1] * scale;
        vec3 v2 = vertices[index2] * scale;
        vec2 uv = (index0 < static_cast<int>(textures.size()))
                      ? textures[index0]
                      : vec2(1, 1);

        auto triangle =
            make_shared<Triangle>(v0, v1, v2, uv, material_triangle);
        triangles.emplace_back(triangle);
    }

    world.add(make_shared<BVHNode>(triangles, 0, triangles.size()));
    world.add(box(point3(-2, 4.5, -2), point3(2, 4.6, 2), difflight));
    // world.add(box(point3(-2, -0.5, -2), point3(2, -0.6, 2),
    // material_ground));

    lights.add(
        box(point3(-2, 4.5, -2), point3(2, 4.6, 2), shared_ptr<Material>()));

    return {world, lights};
}
