#pragma once

#include "point.h"
#include <array>

namespace geometry {

/// 4×4 column-major transformation matrix.
class Transform {
public:
    Transform();                          ///< Identity
    static Transform identity();
    static Transform translation(double tx, double ty, double tz);
    static Transform scale(double sx, double sy, double sz);
    static Transform rotationX(double radians);
    static Transform rotationY(double radians);
    static Transform rotationZ(double radians);

    Transform operator*(const Transform& other) const;
    Point     apply(const Point& p)             const;
    Transform inverse()                         const;

private:
    // Row-major storage: m[row][col]
    std::array<std::array<double, 4>, 4> m;

    explicit Transform(const std::array<std::array<double, 4>, 4>& data);
};

} // namespace geometry
