#pragma once

#include <cmath>
#include <ostream>

namespace geometry {

/// A point (or vector) in 3-D space.
class Point {
public:
    explicit Point(double x = 0.0, double y = 0.0, double z = 0.0);

    double x() const noexcept { return m_x; }
    double y() const noexcept { return m_y; }
    double z() const noexcept { return m_z; }

    double length()              const noexcept;
    double distanceTo(const Point& other) const noexcept;
    Point  normalized()          const;

    Point operator+(const Point& other) const noexcept;
    Point operator-(const Point& other) const noexcept;
    Point operator*(double scalar)      const noexcept;
    double dot(const Point& other)      const noexcept;
    Point  cross(const Point& other)    const noexcept;

    bool operator==(const Point& other) const noexcept;
    bool operator!=(const Point& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Point& p);

private:
    double m_x, m_y, m_z;
};

} // namespace geometry
