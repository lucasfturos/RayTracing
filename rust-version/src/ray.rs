extern crate nalgebra as na;

use na::{Point3, Vector3};

#[derive(Default, Clone)]
pub struct Ray {
    orig: Point3<f64>,
    dir: Vector3<f64>,
    tm: f64,
}

#[allow(dead_code)]
impl Ray {
    pub fn new(origin: Point3<f64>, direction: Vector3<f64>) -> Self {
        Ray {
            orig: origin,
            dir: direction,
            tm: 0.0,
        }
    }

    pub fn with_time(mut self, time: f64) -> Self {
        self.tm = time;
        self
    }

    pub fn origin(&self) -> &Point3<f64> {
        &self.orig
    }

    pub fn direction(&self) -> &Vector3<f64> {
        &self.dir
    }

    pub fn time(&self) -> &f64 {
        &self.tm
    }

    pub fn at(&self, t: f64) -> Point3<f64> {
        self.orig + t * self.dir
    }
}
