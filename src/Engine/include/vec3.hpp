#pragma once

#include <cmath>
#include <iostream>
#include <random>

using std::fabs;
using std::sqrt;

inline double random_double() {
    // Returns a random real in [0,1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max + 0));
}

class vec3 {
  public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e.at(0); }
    double y() const { return e.at(1); }
    double z() const { return e.at(2); }

    vec3 operator-() const { return vec3(-e.at(0), -e.at(1), -e.at(2)); }
    double operator[](int i) const { return e.at(i); }
    double &operator[](int i) { return e.at(i); }

    vec3 &operator+=(const vec3 &v) {
        e.at(0) += v.e.at(0);
        e.at(1) += v.e.at(1);
        e.at(2) += v.e.at(2);
        return *this;
    }

    vec3 &operator*=(const double t) {
        e.at(0) *= t;
        e.at(1) *= t;
        e.at(2) *= t;
        return *this;
    }

    vec3 &operator/=(const double t) { return *this *= 1 / t; }

    double length() const { return sqrt(length_squared()); }

    double length_squared() const {
        return e.at(0) * e.at(0) + e.at(1) * e.at(1) + e.at(2) * e.at(2);
    }

    bool near_zero() const {
        // Retorna verdadeiro se o vetor estiver próximo de zero em todas as
        // dimensões.
        const auto s = 1e-8;
        return (fabs(e.at(0)) < s) && (fabs(e.at(1)) < s) &&
               (fabs(e.at(2)) < s);
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max),
                    random_double(min, max));
    }

  public:
    std::vector<double> e{3};
};

// Tipos de aliases para vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 funções usuais

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e.at(0) << ' ' << v.e.at(1) << ' ' << v.e.at(2);
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e.at(0) + v.e.at(0), u.e.at(1) + v.e.at(1),
                u.e.at(2) + v.e.at(2));
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e.at(0) - v.e.at(0), u.e.at(1) - v.e.at(1),
                u.e.at(2) - v.e.at(2));
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e.at(0) * v.e.at(0), u.e.at(1) * v.e.at(1),
                u.e.at(2) * v.e.at(2));
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.e.at(0), t * v.e.at(1), t * v.e.at(2));
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(const vec3 &v, double t) { return (1 / t) * v; }

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e.at(0) * v.e.at(0) + u.e.at(1) * v.e.at(1) +
           u.e.at(2) * v.e.at(2);
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e.at(1) * v.e.at(2) - u.e.at(2) * v.e.at(1),
                u.e.at(2) * v.e.at(0) - u.e.at(0) * v.e.at(2),
                u.e.at(0) * v.e.at(1) - u.e.at(1) * v.e.at(0));
}

inline vec3 normalize(vec3 v) {
    double mag = v.length();
    return vec3(v.x() / mag, v.y() / mag, v.z() / mag);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p{vec3::random(-1, 1)};
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3 &normal) {
    vec3 in_unit_sphere{random_in_unit_sphere()};
    if (dot(in_unit_sphere, normal) > .0) {
        return -in_unit_sphere;
    } else {
        return in_unit_sphere;
    }
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
