mod camera;
mod color;
mod hitrecord;
mod hittable;
mod hittable_list;
mod interval;
mod material;
mod ray;
mod raytracing;
mod sphere;

use camera::Camera;
use hittable_list::HittableList;
use material::{Dielectric, Lambertian, Metal};
use raytracing::{Arc, Color, Point3, Rc};
use sphere::Sphere;

fn main() {
    let cam = Camera::new(16.0 / 9.0, 400, 30, 10);

    let material_ground = Arc::new(Lambertian::new(Color::new(0.8, 0.8, 0.0)));
    let material_center = Arc::new(Lambertian::new(Color::new(0.1, 0.2, 0.5)));
    let material_left = Arc::new(Dielectric::new(1.5));
    let material_bubble = Arc::new(Dielectric::new(1.0 / 1.5));
    let material_right = Arc::new(Metal::new(Color::new(0.8, 0.6, 0.2), 1.0));

    let mut world = HittableList::new();
    world.add(Rc::new(Sphere::new(
        Point3::new(0.0, 0.0, -1.0),
        0.5,
        material_center.clone(),
    )));
    world.add(Rc::new(Sphere::new(
        Point3::new(0.0, -10.5, -1.0),
        10.0,
        material_ground.clone(),
    )));
    world.add(Rc::new(Sphere::new(
        Point3::new(-1.0, 0.0, -1.0),
        0.5,
        material_left.clone(),
    )));
    world.add(Rc::new(Sphere::new(
        Point3::new(-1.0, 0.0, -1.0),
        0.4,
        material_bubble.clone(),
    )));
    world.add(Rc::new(Sphere::new(
        Point3::new(1.0, 0.0, -1.0),
        0.5,
        material_right.clone(),
    )));

    cam.render(&world);
}

// https://raytracing.github.io/books/RayTracingInOneWeekend.html#positionablecamera
