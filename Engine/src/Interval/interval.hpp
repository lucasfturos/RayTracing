#pragma once

#include <limits>

const double infinity{std::numeric_limits<double>::infinity()};

class Interval {
  public:
    double min, max;
    static const Interval empty, universe;

    Interval();
    Interval(double min, double max);
    Interval(const Interval &a, const Interval &b);

    Interval expand(double delta) const;

    double size() const;
    double clamp(double x) const;

    bool contains(double x) const;
    bool surrounds(double x) const;
};

inline Interval operator+(const Interval &ival, double displacement) {
    return Interval(ival.min + displacement, ival.max + displacement);
}

inline Interval operator+(double displacement, const Interval &ival) {
    return ival + displacement;
}
