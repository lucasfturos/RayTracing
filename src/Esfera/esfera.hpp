#pragma once

#include "BVH/bvh.hpp"
#include "render/material.hpp"

class Esfera {
  private:
    // Material
    shared_ptr<Metal> material_metal;
    shared_ptr<DiffuseLight> difflight;
    shared_ptr<Dielectric> material_dieletric;
    shared_ptr<Lambertian> material_lambertian;
    shared_ptr<Lambertian> material_lambertian_checker;
    shared_ptr<Lambertian> material_lambertian_pertext;

    // Caixa
    shared_ptr<HitTable> box_;

    HittableList world;
    HittableList lights;

  public:
    Scene solar_scene();
    Scene simple_light();
    Scene single_scene();
    Scene random_scene();
};
