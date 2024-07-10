extern crate nalgebra as na;

use na::{Point3, Vector3};

#[derive(Default, Clone)]
pub struct Ray {
    orig: Point3<f64>,
    dir: Vector3<f64>,
}

impl Ray {
    pub fn new(origin: Point3<f64>, direction: Vector3<f64>) -> Self {
        Ray {
            orig: origin,
            dir: direction,
        }
    }

    pub fn origin(&self) -> &Point3<f64> {
        &self.orig
    }

    pub fn direction(&self) -> &Vector3<f64> {
        &self.dir
    }

    pub fn at(&self, t: f64) -> Point3<f64> {
        self.orig + t * self.dir
    }
}
