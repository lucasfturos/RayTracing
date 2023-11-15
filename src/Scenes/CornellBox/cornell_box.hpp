#pragma once

#include "../../Engine/include/perlin.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Box/box.hpp"
#include "../../Engine/src/Constant_Medium/constant_medium.hpp"
#include "../../Engine/src/Sphere/sphere.hpp"
#include "../../Engine/src/Texture/texture.hpp"

class CornellBox {
  private:
    // Material
    shared_ptr<hittable> box1;
    shared_ptr<hittable> box2;
    shared_ptr<metal> metal_tex;
    shared_ptr<lambertian> red;
    shared_ptr<lambertian> white;
    shared_ptr<lambertian> green;
    shared_ptr<lambertian> blue;
    shared_ptr<diffuse_light> light;

  public:
    bvh_node cornell_box();
    bvh_node cornell_smoke();
};
