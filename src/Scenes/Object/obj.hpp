#pragma once

#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/ReadObjectFile/read_object_file.hpp"
#include "../../Engine/include/material.hpp"

class Object {
    // Material
    shared_ptr<metal> material_metal;
    shared_ptr<diffuse_light> difflight;
    shared_ptr<lambertian> material_lambertian;

    shared_ptr<ReadObjectFile> read_object_ptr;

  public:
    Object(const std::string &filepath);

    bvh_node single_scene();
    // bvh_node simple_light();
};
