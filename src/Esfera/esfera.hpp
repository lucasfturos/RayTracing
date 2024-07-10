#pragma once

#include "../../Engine/include/material.hpp"
#include "../../Engine/src/BVH/bvh.hpp"
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

    hittable_list world;
    hittable_list lights;

  public:
    Scene solar_scene();
    Scene simple_light();
    Scene single_scene();
    Scene random_scene();
};
