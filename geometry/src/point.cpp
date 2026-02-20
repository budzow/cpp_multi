#include "geometry/point.h"
#include <cmath>
#include <stdexcept>

namespace geometry {

Point::Point(double x, double y, double z)
    : m_x(x), m_y(y), m_z(z) {}

double Point::length() const noexcept {
    return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

double Point::distanceTo(const Point& other) const noexcept {
    const double dx = m_x - other.m_x;
    const double dy = m_y - other.m_y;
    const double dz = m_z - other.m_z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

Point Point::normalized() const {
    const double len = length();
    if (len == 0.0) {
        throw std::domain_error("Cannot normalise a zero-length vector");
    }
    return Point(m_x / len, m_y / len, m_z / len);
}

Point Point::operator+(const Point& other) const noexcept {
    return Point(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Point Point::operator-(const Point& other) const noexcept {
    return Point(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

Point Point::operator*(double scalar) const noexcept {
    return Point(m_x * scalar, m_y * scalar, m_z * scalar);
}

double Point::dot(const Point& other) const noexcept {
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

Point Point::cross(const Point& other) const noexcept {
    return Point(
        m_y * other.m_z - m_z * other.m_y,
        m_z * other.m_x - m_x * other.m_z,
        m_x * other.m_y - m_y * other.m_x
    );
}

bool Point::operator==(const Point& other) const noexcept {
    constexpr double eps = 1e-9;
    return std::abs(m_x - other.m_x) < eps &&
           std::abs(m_y - other.m_y) < eps &&
           std::abs(m_z - other.m_z) < eps;
}

bool Point::operator!=(const Point& other) const noexcept {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.m_x << ", " << p.m_y << ", " << p.m_z << ")";
    return os;
}

} // namespace geometry
