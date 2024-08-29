#pragma once

#include "BVH/bvh.hpp"
#include "ReadObjectFile/read_object_file.hpp"
#include "render/material.hpp"

class Object {
    // Read Object File
    shared_ptr<ReadObjectFile> read_object_ptr;
    HittableList world;
    HittableList lights;

  public:
    Object(const std::string &filepath);

    Scene single_scene();
};
