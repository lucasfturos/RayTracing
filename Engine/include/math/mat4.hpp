#pragma once

#include "vec3.hpp"
#include <iomanip>

template <typename T> class Mat4 {
  public:
    std::vector<std::vector<T>> elements;

    Mat4() : elements(4, std::vector<T>(4, 0)) {}

    Mat4(T diagonal) : elements(4, std::vector<T>(4, 0)) {
        for (int i = 0; i < 4; ++i) {
            elements[i][i] = diagonal;
        }
    }

    Mat4(const std::vector<std::vector<T>> &elems) : elements(elems) {}

    Mat4(const Vec3<T> &x, const Vec3<T> &y, const Vec3<T> &z, const Vec3<T> &w)
        : elements{{x.x, y.x, z.x, w.x},
                   {x.y, y.y, z.y, w.y},
                   {x.z, y.z, z.z, w.z},
                   {0, 0, 0, 1}} {}

    Mat4(const Vec3<T> &eye, const Vec3<T> &center, const Vec3<T> &up) {
        Vec3<T> forward = unitVector(center - eye);
        Vec3<T> right = unitVector(cross(up, forward));
        Vec3<T> new_up = cross(forward, right);

        elements = {{right.x, right.y, right.z, -dot(right, eye)},
                    {new_up.x, new_up.y, new_up.z, -dot(new_up, eye)},
                    {-forward.x, -forward.y, -forward.z, dot(forward, eye)},
                    {0, 0, 0, 1}};
    }

    T &operator()(int row, int col) { return elements[row][col]; }

    const T &operator()(int row, int col) const { return elements[row][col]; }

    Mat4<T> operator*(const Mat4<T> &other) const {
        Mat4<T> result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result(row, col) = 0;
                for (int i = 0; i < 4; ++i) {
                    result(row, col) += (*this)(row, i) * other(i, col);
                }
            }
        }
        return result;
    }

    Mat4<T> operator*(T scalar) const {
        Mat4<T> result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) = (*this)(i, j) * scalar;
            }
        }
        return result;
    }

    Mat4<T> operator/(T scalar) const {
        Mat4<T> result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) = (*this)(i, j) / scalar;
            }
        }
        return result;
    }

    Vec3<T> colToVec3(int col) const {
        return Vec3<T>{elements[0][col], elements[1][col], elements[2][col]};
    }

    Mat4<T> transpose() const {
        Mat4<T> result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result(col, row) = (*this)(row, col);
            }
        }
        return result;
    }

    static Mat4<T> identity() { return Mat4<T>(1); }

    static Mat4<T> translation(const Vec3<T> &translation) {
        Mat4<T> result = identity();
        result(0, 3) = translation.x;
        result(1, 3) = translation.y;
        result(2, 3) = translation.z;
        return result;
    }

    static Mat4<T> scaling(const Vec3<T> &scale) {
        Mat4<T> result = identity();
        result(0, 0) = scale.x;
        result(1, 1) = scale.y;
        result(2, 2) = scale.z;
        return result;
    }

    static Mat4<T> rotationZ(T angle) {
        Mat4<T> result = identity();
        T c = std::cos(angle);
        T s = std::sin(angle);
        result(0, 0) = c;
        result(0, 1) = -s;
        result(1, 0) = s;
        result(1, 1) = c;
        return result;
    }

    static Mat4<T> perspective(T fov, T aspect, T near, T far) {
        Mat4<T> result;
        T tanHalfFov = std::tan(fov / 2);

        result(0, 0) = 1 / (aspect * tanHalfFov);
        result(1, 1) = 1 / (tanHalfFov);
        result(2, 2) = -(far + near) / (far - near);
        result(2, 3) = -(2 * far * near) / (far - near);
        result(3, 2) = -1;
        return result;
    }

    Mat4<T> normalize() const {
        Mat4<T> result;
        T length = std::sqrt(
            result(0, 0) * result(0, 0) + result(0, 1) * result(0, 1) +
            result(0, 2) * result(0, 2) + result(0, 3) * result(0, 3) +
            result(1, 0) * result(1, 0) + result(1, 1) * result(1, 1) +
            result(1, 2) * result(1, 2) + result(1, 3) * result(1, 3) +
            result(2, 0) * result(2, 0) + result(2, 1) * result(2, 1) +
            result(2, 2) * result(2, 2) + result(2, 3) * result(2, 3) +
            result(3, 0) * result(3, 0) + result(3, 1) * result(3, 1) +
            result(3, 2) * result(3, 2) + result(3, 3) * result(3, 3));
        if (length > 0) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result(i, j) /= length;
                }
            }
        }
        return result;
    }

    T determinant() const {
        T det = 0;
        for (int i = 0; i < 4; ++i) {
            det += (*this)(0, i) * cofactor(0, i);
        }
        return det;
    }

    Mat4<T> adjugate() const {
        Mat4<T> adj;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                adj(j, i) = cofactor(i, j);
            }
        }
        return adj;
    }

    Mat4<T> inverse() const {
        T det = determinant();
        if (det == 0) {
            throw std::runtime_error(
                "Matrix is singular and cannot be inverted.");
        }
        return adjugate() / det;
    }

  private:
    T cofactor(int row, int col) const {
        return ((row + col) % 2 == 0 ? 1 : -1) * minor(row, col);
    }

    T minor(int row, int col) const {
        Vec3<T> row1, row2, row3;
        int subi = 0;

        for (int i = 0; i < 4; ++i) {
            if (i == row)
                continue;
            int subj = 0;
            for (int j = 0; j < 4; ++j) {
                if (j == col)
                    continue;
                if (subi == 0)
                    row1[subj] = (*this)(i, j);
                else if (subi == 1)
                    row2[subj] = (*this)(i, j);
                else
                    row3[subj] = (*this)(i, j);
                ++subj;
            }
            ++subi;
        }

        return row1.x * (row2.y * row3.z - row2.z * row3.y) -
               row1.y * (row2.x * row3.z - row2.z * row3.x) +
               row1.z * (row2.x * row3.y - row2.y * row3.x);
    }
};

// Tipos de aliases
using mat4 = Mat4<double>;

// Função para normalizar matrizes
template <typename T> inline Mat4<T> normalize(const Mat4<T> &m) {
    Mat4<T> result = m;
    T length =
        std::sqrt(m(0, 0) * m(0, 0) + m(0, 1) * m(0, 1) + m(0, 2) * m(0, 2) +
                  m(0, 3) * m(0, 3) + m(1, 0) * m(1, 0) + m(1, 1) * m(1, 1) +
                  m(1, 2) * m(1, 2) + m(1, 3) * m(1, 3) + m(2, 0) * m(2, 0) +
                  m(2, 1) * m(2, 1) + m(2, 2) * m(2, 2) + m(2, 3) * m(2, 3) +
                  m(3, 0) * m(3, 0) + m(3, 1) * m(3, 1) + m(3, 2) * m(3, 2) +
                  m(3, 3) * m(3, 3));
    if (length > 0) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) /= length;
            }
        }
    }
    return result;
}

template <typename T> inline void print(const Mat4<T> &matrix) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            T value = matrix(i, j);
            if (std::abs(value) < std::numeric_limits<T>::epsilon()) {
                value = 0;
            }
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::defaultfloat;
}
