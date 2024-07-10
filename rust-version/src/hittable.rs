pub use crate::hitrecord::HitRecord;
use crate::raytracing::{Interval, Ray};

pub trait Hittable {
    fn hit(&self, r: &Ray, ray_t: Interval, rec: &mut HitRecord) -> bool;
}
