#include "geometry/shape.h"
#include <cmath>
#include <stdexcept>

namespace geometry {

static constexpr double PI = 3.14159265358979323846;

// ── Circle ───────────────────────────────────────────────────────────────────

Circle::Circle(const Point& center, double radius)
    : m_center(center), m_radius(radius)
{
    if (radius <= 0.0) {
        throw std::invalid_argument("Circle radius must be positive");
    }
}

double      Circle::area()      const { return PI * m_radius * m_radius; }
double      Circle::perimeter() const { return 2.0 * PI * m_radius; }
std::string Circle::name()      const { return "Circle"; }
Point       Circle::centroid()  const { return m_center; }

// ── Triangle ─────────────────────────────────────────────────────────────────

Triangle::Triangle(const Point& a, const Point& b, const Point& c)
    : m_a(a), m_b(b), m_c(c) {}

double Triangle::area() const {
    // Heron's formula
    const double ab = m_a.distanceTo(m_b);
    const double bc = m_b.distanceTo(m_c);
    const double ca = m_c.distanceTo(m_a);
    const double s  = (ab + bc + ca) / 2.0;
    return std::sqrt(s * (s - ab) * (s - bc) * (s - ca));
}

double Triangle::perimeter() const {
    return m_a.distanceTo(m_b) + m_b.distanceTo(m_c) + m_c.distanceTo(m_a);
}

std::string Triangle::name() const { return "Triangle"; }

Point Triangle::centroid() const {
    return Point(
        (m_a.x() + m_b.x() + m_c.x()) / 3.0,
        (m_a.y() + m_b.y() + m_c.y()) / 3.0,
        (m_a.z() + m_b.z() + m_c.z()) / 3.0
    );
}

// ── Rectangle ────────────────────────────────────────────────────────────────

Rectangle::Rectangle(const Point& origin, double width, double height)
    : m_origin(origin), m_width(width), m_height(height)
{
    if (width <= 0.0 || height <= 0.0) {
        throw std::invalid_argument("Rectangle dimensions must be positive");
    }
}

double      Rectangle::area()      const { return m_width * m_height; }
double      Rectangle::perimeter() const { return 2.0 * (m_width + m_height); }
std::string Rectangle::name()      const { return "Rectangle"; }

Point Rectangle::centroid() const {
    return Point(
        m_origin.x() + m_width  / 2.0,
        m_origin.y() + m_height / 2.0,
        m_origin.z()
    );
}

} // namespace geometry
