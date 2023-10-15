#pragma once

#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Box/box.hpp"
#include "../../Engine/src/Triangle/triangle.hpp"

class BuleUtah {
    // Material
    shared_ptr<metal> material_metal;
    shared_ptr<diffuse_light> difflight;
    shared_ptr<lambertian> material_lambertian;

  public:
    bvh_node single_scene();
    // bvh_node simple_light();
};
