#pragma once

#include "BVH/bvh.hpp"
#include "render/material.hpp"

class CornellBox {
  private:
    // Material
    shared_ptr<Metal> metal_tex;
    shared_ptr<Lambertian> red;
    shared_ptr<Lambertian> white;
    shared_ptr<Lambertian> green;
    shared_ptr<Lambertian> blue;
    shared_ptr<DiffuseLight> light;

  public:
    Scene cornell_box();
    Scene cornell_smoke();
    Scene final_scene();
};
