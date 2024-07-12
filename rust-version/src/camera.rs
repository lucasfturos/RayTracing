use crate::hittable::{HitRecord, Hittable};
use crate::raytracing::*;

pub struct Camera {
    image_width: usize,
    image_height: usize,
    samples_per_pixel: usize,
    max_depth: usize,
    center: Point3<f64>,
    pixel00_loc: Point3<f64>,
    pixel_delta_u: Vector3<f64>,
    pixel_delta_v: Vector3<f64>,
    pixel_samples_scale: f64,
    defocus_angle: f64,
    defocus_disk_u: Vector3<f64>,
    defocus_disk_v: Vector3<f64>,
}

impl Camera {
    pub fn new(
        aspect_ratio: f64,
        image_width: usize,
        samples_per_pixel: usize,
        max_depth: usize,
        vfov: f64,
        lookfrom: Point3<f64>,
        lookat: Point3<f64>,
        vup: Vector3<f64>,
        defocus_angle: f64,
        focus_dist: f64,
    ) -> Self {
        let image_height = (image_width as f64 / aspect_ratio) as usize;
        let image_height = image_height;

        let pixel_samples_scale = 1.0 / samples_per_pixel as f64;

        let center = lookfrom;

        // Determine viewport dimensions.
        let theta = degrees_to_radians(vfov);
        let h = (theta / 2.0).tan();
        let viewport_height = 2.0 * h * focus_dist;
        let viewport_width = viewport_height * (image_width as f64 / image_height as f64);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        let w = unit_vector(lookfrom - lookat);
        let u = unit_vector(vup.cross(&w));
        let v = w.cross(&u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        let viewport_u = viewport_width * u;
        let viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        let pixel_delta_u = viewport_u / image_width as f64;
        let pixel_delta_v = viewport_v / image_height as f64;

        // Calculate the location of the upper left pixel.
        let viewport_upper_left = center - (focus_dist * w) - viewport_u / 2.0 - viewport_v / 2.0;
        let pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        let defocus_radius = focus_dist * (degrees_to_radians(defocus_angle / 2.0)).tan();
        let defocus_disk_u = u * defocus_radius;
        let defocus_disk_v = v * defocus_radius;

        Self {
            image_width,
            image_height,
            samples_per_pixel,
            max_depth,
            center,
            pixel00_loc,
            pixel_delta_u,
            pixel_delta_v,
            pixel_samples_scale,
            defocus_angle,
            defocus_disk_u,
            defocus_disk_v,
        }
    }

    pub fn ray_color(&self, r: &Ray, depth: usize, world: &dyn Hittable) -> Color {
        if depth <= 0 {
            return Color::new(0.0, 0.0, 0.0);
        }

        let mut rec = HitRecord::default();
        if world.hit(r, Interval::new(0.001, INFINITY), &mut rec) {
            let mut scattered = Ray::default();
            let mut attenuation = Color::new(0.0, 0.0, 0.0);
            if let Some(material) = &rec.mat {
                if material.scatter(r, &rec, &mut attenuation, &mut scattered) {
                    return attenuation.component_mul(&self.ray_color(
                        &scattered,
                        depth - 1,
                        world,
                    ));
                }
                return Color::new(0.0, 0.0, 0.0);
            }
        }

        let unit_direction = r.direction().normalize();
        let a = 0.5 * (unit_direction.y + 1.0);
        (1.0 - a) * Color::new(1.0, 1.0, 1.0) + a * Color::new(0.5, 0.7, 1.0)
    }

    fn get_ray(&self, i: usize, j: usize) -> Ray {
        let offset = self.sample_square();
        let pixel_sample = self.pixel00_loc
            + (i as f64 + offset.x) * self.pixel_delta_u
            + (j as f64 + offset.y) * self.pixel_delta_v;

        let ray_origin = if self.defocus_angle <= 0.0 {
            self.center
        } else {
            self.defocus_disk_sample()
        };
        let ray_direction = pixel_sample - ray_origin;

        Ray::new(ray_origin, ray_direction.normalize())
    }

    fn sample_square(&self) -> Vector3<f64> {
        Vector3::new(random_double() - 0.5, random_double() - 0.5, 0.0)
    }

    fn defocus_disk_sample(&self) -> Point3<f64> {
        let p = random_in_unit_disk();
        self.center + (p.x * self.defocus_disk_u) + (p.y * self.defocus_disk_v)
    }

    pub fn render(&self, world: &dyn Hittable) {
        println!("P3\n{} {}\n255", self.image_width, self.image_height);

        for j in 0..self.image_height {
            eprint!("\rScanlines remaining: {} ", self.image_height - j - 1);
            for i in 0..self.image_width {
                let mut pixel_color = Color::new(0.0, 0.0, 0.0);
                for _ in 0..self.samples_per_pixel {
                    let r = self.get_ray(i, j);
                    pixel_color += self.ray_color(&r, self.max_depth, world);
                }
                write_color(
                    &mut std::io::stdout(),
                    self.pixel_samples_scale * pixel_color,
                )
                .unwrap();
            }
        }

        eprint!("\n\rDone.\n");
    }
}
