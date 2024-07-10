use crate::material::Material;
use crate::raytracing::{Arc, Point3, Ray, Vector3};

#[derive(Default, Clone)]
pub struct HitRecord {
    pub p: Point3<f64>,
    pub normal: Vector3<f64>,
    pub mat: Option<Arc<dyn Material + Send + Sync>>,
    pub t: f64,
    pub front_face: bool,
}

impl HitRecord {
    pub fn set_face_normal(&mut self, r: &Ray, outward_normal: &Vector3<f64>) {
        self.front_face = r.direction().dot(&outward_normal) < 0.0;
        self.normal = if self.front_face {
            *outward_normal
        } else {
            -*outward_normal
        };
    }
}
