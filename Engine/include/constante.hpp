#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>

// Constants
const double infinity{std::numeric_limits<double>::infinity()};
const double pi{3.1415926535897932385};

// Utility Functions

template <typename T> inline T degrees_to_radians(T degrees) {
    return degrees * pi / 180.0;
}

template <typename T> inline T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

// Using
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Header

#include "ray.hpp"
#include "vec3.hpp"
