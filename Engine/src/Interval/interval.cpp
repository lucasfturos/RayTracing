#include "interval.hpp"
#include <algorithm>

const interval interval::empty = interval(infinity, -infinity);
const interval interval::universe = interval(-infinity, infinity);

interval::interval() : min(+infinity), max(-infinity) {}

interval::interval(double min, double max) : min(min), max(max) {}

interval::interval(const interval &a, const interval &b) {
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
}

interval interval::expand(double delta) const {
    auto padding = delta / 2;
    return interval(min - padding, max + padding);
}

double interval::size() const { return max - min; }

bool interval::contains(double x) const { return min <= x && x <= max; }

bool interval::surrounds(double x) const { return min < x && x < max; }

double interval::clamp(double x) const { return std::clamp(x, min, max); }
