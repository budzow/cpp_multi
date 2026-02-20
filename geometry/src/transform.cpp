#include "geometry/transform.h"
#include <cmath>
#include <stdexcept>

namespace geometry {

// ── helpers ──────────────────────────────────────────────────────────────────

using Mat4 = std::array<std::array<double, 4>, 4>;

static Mat4 zero4x4() {
    Mat4 m{};
    for (auto& row : m) row.fill(0.0);
    return m;
}

static Mat4 identity4x4() {
    Mat4 m = zero4x4();
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0;
    return m;
}

// ── constructors / factories ─────────────────────────────────────────────────

Transform::Transform() : m(identity4x4()) {}

Transform::Transform(const Mat4& data) : m(data) {}

Transform Transform::identity() { return Transform{}; }

Transform Transform::translation(double tx, double ty, double tz) {
    Mat4 d = identity4x4();
    d[0][3] = tx;
    d[1][3] = ty;
    d[2][3] = tz;
    return Transform(d);
}

Transform Transform::scale(double sx, double sy, double sz) {
    Mat4 d = identity4x4();
    d[0][0] = sx;
    d[1][1] = sy;
    d[2][2] = sz;
    return Transform(d);
}

Transform Transform::rotationX(double r) {
    Mat4 d = identity4x4();
    d[1][1] =  std::cos(r);  d[1][2] = -std::sin(r);
    d[2][1] =  std::sin(r);  d[2][2] =  std::cos(r);
    return Transform(d);
}

Transform Transform::rotationY(double r) {
    Mat4 d = identity4x4();
    d[0][0] =  std::cos(r);  d[0][2] =  std::sin(r);
    d[2][0] = -std::sin(r);  d[2][2] =  std::cos(r);
    return Transform(d);
}

Transform Transform::rotationZ(double r) {
    Mat4 d = identity4x4();
    d[0][0] =  std::cos(r);  d[0][1] = -std::sin(r);
    d[1][0] =  std::sin(r);  d[1][1] =  std::cos(r);
    return Transform(d);
}

// ── operations ───────────────────────────────────────────────────────────────

Transform Transform::operator*(const Transform& other) const {
    Mat4 res = zero4x4();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                res[i][j] += m[i][k] * other.m[k][j];
    return Transform(res);
}

Point Transform::apply(const Point& p) const {
    const double w = m[3][0]*p.x() + m[3][1]*p.y() + m[3][2]*p.z() + m[3][3];
    return Point(
        (m[0][0]*p.x() + m[0][1]*p.y() + m[0][2]*p.z() + m[0][3]) / w,
        (m[1][0]*p.x() + m[1][1]*p.y() + m[1][2]*p.z() + m[1][3]) / w,
        (m[2][0]*p.x() + m[2][1]*p.y() + m[2][2]*p.z() + m[2][3]) / w
    );
}

/// Gauss-Jordan inverse for a 4×4 matrix.
Transform Transform::inverse() const {
    // Augment [m | I]
    std::array<std::array<double, 8>, 4> aug{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) aug[i][j] = m[i][j];
        aug[i][4 + i] = 1.0;
    }
    for (int col = 0; col < 4; ++col) {
        // Pivot
        int pivot = col;
        for (int row = col + 1; row < 4; ++row)
            if (std::abs(aug[row][col]) > std::abs(aug[pivot][col]))
                pivot = row;
        std::swap(aug[col], aug[pivot]);

        if (std::abs(aug[col][col]) < 1e-12)
            throw std::runtime_error("Transform matrix is singular");

        const double div = aug[col][col];
        for (double& v : aug[col]) v /= div;

        for (int row = 0; row < 4; ++row) {
            if (row == col) continue;
            const double factor = aug[row][col];
            for (int j = 0; j < 8; ++j)
                aug[row][j] -= factor * aug[col][j];
        }
    }
    Mat4 result{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result[i][j] = aug[i][4 + j];
    return Transform(result);
}

} // namespace geometry
