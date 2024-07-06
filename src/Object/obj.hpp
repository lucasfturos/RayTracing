#pragma once

#include "../../Engine/include/material.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/ReadObjectFile/read_object_file.hpp"

class Object {
    // Read Object File
    shared_ptr<ReadObjectFile> read_object_ptr;

  public:
    Object(const std::string &filepath);

    bvh_node single_scene();
    // bvh_node simple_light();
};
