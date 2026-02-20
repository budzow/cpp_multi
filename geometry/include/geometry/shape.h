#pragma once

#include "point.h"
#include <string>
#include <vector>
#include <memory>

namespace geometry {

/// Abstract base for all 2-D / 3-D shapes.
class Shape {
public:
    virtual ~Shape() = default;

    virtual double      area()      const = 0;
    virtual double      perimeter() const = 0;
    virtual std::string name()      const = 0;
    virtual Point       centroid()  const = 0;
};

// ─────────────────────────────────────────────────────────────────────────────

class Circle : public Shape {
public:
    Circle(const Point& center, double radius);

    double      area()      const override;
    double      perimeter() const override;
    std::string name()      const override;
    Point       centroid()  const override;

    const Point& center() const noexcept { return m_center; }
    double       radius() const noexcept { return m_radius; }

private:
    Point  m_center;
    double m_radius;
};

// ─────────────────────────────────────────────────────────────────────────────

class Triangle : public Shape {
public:
    Triangle(const Point& a, const Point& b, const Point& c);

    double      area()      const override;
    double      perimeter() const override;
    std::string name()      const override;
    Point       centroid()  const override;

    const Point& a() const noexcept { return m_a; }
    const Point& b() const noexcept { return m_b; }
    const Point& c() const noexcept { return m_c; }

private:
    Point m_a, m_b, m_c;
};

// ─────────────────────────────────────────────────────────────────────────────

class Rectangle : public Shape {
public:
    /// \p origin is the bottom-left corner; width and height extend in +X / +Y.
    Rectangle(const Point& origin, double width, double height);

    double      area()      const override;
    double      perimeter() const override;
    std::string name()      const override;
    Point       centroid()  const override;

    double width()  const noexcept { return m_width;  }
    double height() const noexcept { return m_height; }

private:
    Point  m_origin;
    double m_width, m_height;
};

} // namespace geometry
