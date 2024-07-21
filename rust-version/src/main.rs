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
use material::{Dielectric, Lambertian, Material, Metal};
use raytracing::*;
use sphere::Sphere;

fn main() {
    let aspect_ratio = 1.0;
    let image_width = 400;
    let samples_per_pixel = 10;
    let max_depth = 10;

    let vfov = 20.0;
    let lookfrom = Point3::new(13.0, 2.0, 3.0);
    let lookat = Point3::new(0.0, 0.0, 0.0);
    let vup = Vector3::new(0.0, 1.0, 0.0);

    let defocus_angle = 0.6;
    let focus_dist = 10.0;

    let cam = Camera::new(
        aspect_ratio,
        image_width,
        samples_per_pixel,
        max_depth,
        vfov,
        lookfrom,
        lookat,
        vup,
        defocus_angle,
        focus_dist,
    );

    let material_ground = Arc::new(Lambertian::new(Color::new(0.5, 0.5, 0.5)));
    let mut world = HittableList::new();

    world.add(Rc::new(Sphere::new(
        Point3::new(0.0, -1000.0, 0.0),
        1000.0,
        material_ground.clone(),
    )));

    let mut sphere_material: Arc<dyn Material + Send + Sync>;
    let range = 2;
    for a in -range..range {
        for b in -range..range {
            let choose_mat = random_double();
            let center = Point3::new(
                a as f64 + 0.9 * random_double(),
                0.2,
                b as f64 + 0.9 * random_double(),
            );
            if (center - Point3::new(4.0, 0.2, 0.0)).norm() > 0.9 {
                if choose_mat < 0.8 {
                    // diffuse
                    let albedo = random_color();
                    sphere_material = Arc::new(Lambertian::new(albedo));
                } else if choose_mat < 0.95 {
                    // metal
                    let albedo = random_color_range(0.5, 1.0);
                    let fuzz = random_double_range(0.0, 0.5);
                    sphere_material = Arc::new(Metal::new(albedo, fuzz));
                } else {
                    // glass
                    sphere_material = Arc::new(Dielectric::new(1.5));
                }
                world.add(Rc::new(Sphere::new(center, 0.2, sphere_material.clone())));
            }
        }
    }

    let material1 = Arc::new(Dielectric::new(1.5));
    world.add(Rc::new(Sphere::new(
        Point3::new(0.0, 1.0, 0.0),
        1.0,
        material1,
    )));

    let material2 = Arc::new(Lambertian::new(Color::new(0.4, 0.2, 0.1)));
    world.add(Rc::new(Sphere::new(
        Point3::new(-4.0, 1.0, 0.0),
        1.0,
        material2,
    )));

    let material3 = Arc::new(Metal::new(Color::new(0.7, 0.6, 0.5), 0.0));
    world.add(Rc::new(Sphere::new(
        Point3::new(4.0, 1.0, 0.0),
        1.0,
        material3,
    )));

    cam.render(&world);
}

// https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies
