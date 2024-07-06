#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <unordered_map>

// Constants
const double pi{3.1415926535897932385};
const double eps = std::numeric_limits<float>::epsilon();

// Utility Functions
template <typename T> inline T degrees_to_radians(T degrees) {
    return degrees * pi / 180.0;
}

template <typename T> inline T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

// Using
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::sqrt;
using std::unique_ptr;
using std::unordered_map;

// Header

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"