// Common Imports
pub use crate::color::{write_color, Color};
pub use crate::interval::Interval;
pub use crate::ray::Ray;
pub extern crate nalgebra as na;
pub use na::{Point3, Vector3};
pub use rand::Rng;
pub use std::{rc::Rc, sync::Arc};

// Constants
pub const INFINITY: f64 = f64::INFINITY;
#[allow(unused_variables)]
pub const PI: f64 = std::f64::consts::PI;

// Utility Functions
pub fn unit_vector(v: Vector3<f64>) -> Vector3<f64> {
    v / v.norm()
}

pub fn near_zero(v: &Vector3<f64>) -> bool {
    const S: f64 = 1e-8;
    v.iter().all(|&x| x.abs() < S)
}

pub fn reflect(v: &Vector3<f64>, n: &Vector3<f64>) -> Vector3<f64> {
    v - 2.0 * v.dot(n) * n
}

pub fn refract(uv: &Vector3<f64>, n: &Vector3<f64>, etai_over_etat: f64) -> Vector3<f64> {
    let cos_theta = -uv.dot(&n).min(1.0);
    let r_out_perp = etai_over_etat * (uv + cos_theta * n);
    let r_out_parallel = -((1.0 - r_out_perp.norm_squared()).abs().sqrt()) * n;
    r_out_perp + r_out_parallel
}

pub fn random_in_unit_sphere() -> Vector3<f64> {
    let mut rng = rand::thread_rng();
    loop {
        let p = 2.0 * Vector3::new(rng.gen::<f64>(), rng.gen::<f64>(), rng.gen::<f64>())
            - Vector3::new(1.0, 1.0, 1.0);
        if p.norm_squared() < 1.0 {
            return p;
        }
    }
}

pub fn random_unit_vector() -> Vector3<f64> {
    unit_vector(random_in_unit_sphere())
}

#[allow(dead_code)]
pub fn random_on_hemisphere(normal: Vector3<f64>) -> Vector3<f64> {
    let on_unit_sphere = random_unit_vector();
    if on_unit_sphere.dot(&normal) > 0.0 {
        on_unit_sphere
    } else {
        -on_unit_sphere
    }
}

#[allow(dead_code)]
pub fn degrees_to_radians(degrees: f64) -> f64 {
    degrees * PI / 180.0
}

pub fn random_double() -> f64 {
    rand::thread_rng().gen::<f64>()
}

#[allow(dead_code)]
pub fn random_double_range(min: f64, max: f64) -> f64 {
    min + (max - min) * random_double()
}
