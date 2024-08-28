#include "interval.hpp"
#include <algorithm>

const Interval Interval::empty = Interval(infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, infinity);

Interval::Interval() : min(+infinity), max(-infinity) {}

Interval::Interval(double min, double max) : min(min), max(max) {}

Interval::Interval(const Interval &a, const Interval &b) {
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
}

Interval Interval::expand(double delta) const {
    auto padding = delta / 2;
    return Interval(min - padding, max + padding);
}

double Interval::size() const { return max - min; }

bool Interval::contains(double x) const { return min <= x && x <= max; }

bool Interval::surrounds(double x) const { return min < x && x < max; }

double Interval::clamp(double x) const { return std::clamp(x, min, max); }
