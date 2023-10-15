#pragma once

#include "../../Engine/include/perlin.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
#include "../../Engine/src/Box/box.hpp"
#include "../../Engine/src/Constant_Medium/constant_medium.hpp"
#include "../../Engine/src/Sphere/sphere.hpp"
#include <SDL2/SDL.h>

class Esfera {
  private:
    // Material
    shared_ptr<metal> material_metal;
    shared_ptr<diffuse_light> difflight;
    shared_ptr<dielectric> material_dieletric;
    shared_ptr<lambertian> material_lambertian;
    shared_ptr<lambertian> material_lambertian_checker;
    shared_ptr<lambertian> material_lambertian_pertext;

    // Caixa
    shared_ptr<hittable> box_;

  public:
    bvh_node solar_scene();
    bvh_node simple_light();
    bvh_node single_scene();
    bvh_node random_scene();
};
