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

inline int random_int() {
    // Returns a random real in [0,1).
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator);
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max + 0));
}

template <typename T> class Vec3 {
  public:
    T x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    T operator[](int i) const { return (&x)[i]; }
    T &operator[](int i) { return (&x)[i]; }

    Vec3 &operator+=(const Vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3 &operator*=(const T t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec3 &operator*=(const Vec3 &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vec3 &operator/=(const T t) { return *this *= 1 / t; }

    T length() const { return sqrt(length_squared()); }

    T length_squared() const { return x * x + y * y + z * z; }

    bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    static Vec3 random(double min, double max) {
        return Vec3(random_double(min, max), random_double(min, max),
                    random_double(min, max));
    }
};

// Tipos de aliases para vec3
using vec3 = Vec3<double>;
using ivec3 = Vec3<int>;
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 funções usuais

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Vec3<T> &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

template <typename T>
inline Vec3<T> operator+(const Vec3<T> &u, const Vec3<T> &v) {
    return Vec3<T>(u.x + v.x, u.y + v.y, u.z + v.z);
}

template <typename T>
inline Vec3<T> operator-(const Vec3<T> &u, const Vec3<T> &v) {
    return Vec3<T>(u.x - v.x, u.y - v.y, u.z - v.z);
}

template <typename T>
inline Vec3<T> operator*(const Vec3<T> &u, const Vec3<T> &v) {
    return Vec3<T>(u.x * v.x, u.y * v.y, u.z * v.z);
}

template <typename T> inline Vec3<T> operator*(double t, const Vec3<T> &v) {
    return Vec3<T>(t * v.x, t * v.y, t * v.z);
}

template <typename T> inline Vec3<T> operator*(const Vec3<T> &v, double t) {
    return t * v;
}

template <typename T> inline Vec3<T> operator/(const Vec3<T> &v, double t) {
    return (1 / t) * v;
}

template <typename T> inline double dot(const Vec3<T> &u, const Vec3<T> &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T> inline Vec3<T> cross(const Vec3<T> &u, const Vec3<T> &v) {
    return Vec3<T>(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z,
                   u.x * v.y - u.y * v.x);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

template <typename T>
inline Vec3<T> random_in_hemisphere(const Vec3<T> &normal) {
    Vec3<T> in_unit_sphere{random_in_unit_sphere()};
    if (dot(in_unit_sphere, normal) > .0) {
        return -in_unit_sphere;
    } else {
        return in_unit_sphere;
    }
}

inline vec3 random_in_unit_disk() {
    while (true) {
        vec3 p(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

template <typename T>
inline Vec3<T> reflect(const Vec3<T> &v, const Vec3<T> &n) {
    return v - 2 * dot(v, n) * n;
}

template <typename T>
inline Vec3<T> refract(const Vec3<T> &uv, const Vec3<T> &n,
                       double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3<T> r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3<T> r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
