extern crate nalgebra as na;
use crate::interval::Interval;

use na::Vector3;
use std::io::{self, Write};

pub type Color = Vector3<f64>;

#[allow(dead_code)]
pub fn linear_to_gamma(linear_component: f64) -> f64 {
    if linear_component > 0.0 {
        return linear_component.sqrt();
    }
    0.0
}

pub fn write_color<W: Write>(out: &mut W, pixel_color: Color) -> io::Result<()> {
    let /* mut */ r = pixel_color.x;
    let /* mut */ g = pixel_color.y;
    let /* mut */ b = pixel_color.z;

    // r = linear_to_gamma(r);
    // g = linear_to_gamma(g);
    // b = linear_to_gamma(b);

    let intensity = Interval::new(0.0, 0.999);
    let rbyte = (256.0 * intensity.clamp(r)) as u8;
    let gbyte = (256.0 * intensity.clamp(g)) as u8;
    let bbyte = (256.0 * intensity.clamp(b)) as u8;

    writeln!(out, "{} {} {}", rbyte, gbyte, bbyte)
}
