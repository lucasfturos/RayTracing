#pragma once

#include "../../Engine/include/material.hpp"
#include "../../Engine/src/BVH/bvh.hpp"

class CornellBox {
  private:
    // Material
    shared_ptr<metal> metal_tex;
    shared_ptr<lambertian> red;
    shared_ptr<lambertian> white;
    shared_ptr<lambertian> green;
    shared_ptr<lambertian> blue;
    shared_ptr<diffuse_light> light;

  public:
    Scene cornell_box();
    Scene cornell_smoke();
    Scene final_scene();
};
