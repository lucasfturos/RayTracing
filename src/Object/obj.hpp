#pragma once

#include "../../Engine/include/material.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/ReadObjectFile/read_object_file.hpp"

class Object {
    // Read Object File
    shared_ptr<ReadObjectFile> read_object_ptr;
    HittableList world;
    HittableList lights;

  public:
    Object(const std::string &filepath);

    Scene single_scene();
};
