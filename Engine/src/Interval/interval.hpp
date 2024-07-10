#pragma once

#include <limits>

const double infinity{std::numeric_limits<double>::infinity()};

class interval {
  public:
    double min, max;
    static const interval empty, universe;

    interval();
    interval(double min, double max);
    interval(const interval &a, const interval &b);

    interval expand(double delta) const;

    double size() const;
    double clamp(double x) const;

    bool contains(double x) const;
    bool surrounds(double x) const;
};

inline interval operator+(const interval &ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

inline interval operator+(double displacement, const interval &ival) {
    return ival + displacement;
}
