#include "geometry/point.h"
#include "geometry/shape.h"
#include "geometry/transform.h"
#include "io/logger.h"
#include "io/file_writer.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <cmath>

// ── helper ────────────────────────────────────────────────────────────────────

static void printShape(const geometry::Shape& s, io::Logger& log) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4)
        << s.name()
        << "  area=" << s.area()
        << "  perimeter=" << s.perimeter();
    log.info(oss.str());
}

// ── main ──────────────────────────────────────────────────────────────────────

int main() {
    auto& log = io::Logger::instance();
    log.setLevel(io::LogLevel::DEBUG);

    log.info("=== Geometry demo starting ===");

    // ── Points & vector algebra ───────────────────────────────────────────────
    log.debug("Creating points");

    const geometry::Point origin(0.0, 0.0, 0.0);
    const geometry::Point p1(3.0, 4.0, 0.0);
    const geometry::Point p2(6.0, 0.0, 0.0);
    const geometry::Point p3(1.0, 1.0, 1.0);

    std::cout << "\n-- Points --\n";
    std::cout << "origin = " << origin << "\n";
    std::cout << "p1     = " << p1     << "  |p1| = " << p1.length()             << "\n";
    std::cout << "p2     = " << p2     << "\n";
    std::cout << "p1+p2  = " << (p1 + p2) << "\n";
    std::cout << "p1·p2  = " << p1.dot(p2) << "\n";
    std::cout << "p1×p3  = " << p1.cross(p3) << "\n";
    std::cout << "dist(origin, p1) = " << origin.distanceTo(p1) << "\n";

    // ── Shapes ────────────────────────────────────────────────────────────────
    log.debug("Building shapes");

    std::vector<std::unique_ptr<geometry::Shape>> shapes;
    shapes.push_back(std::make_unique<geometry::Circle>(origin, 5.0));
    shapes.push_back(std::make_unique<geometry::Triangle>(origin, p1, p2));
    shapes.push_back(std::make_unique<geometry::Rectangle>(origin, 4.0, 3.0));

    std::cout << "\n-- Shapes --\n";
    for (const auto& s : shapes) {
        printShape(*s, log);
        std::cout << "  centroid = " << s->centroid() << "\n";
    }

    // ── Transforms ────────────────────────────────────────────────────────────
    log.debug("Applying transforms");

    const geometry::Transform t1 = geometry::Transform::translation(1.0, 2.0, 3.0);
    const geometry::Transform rx = geometry::Transform::rotationX(M_PI / 4.0); // 45°
    const geometry::Transform combined = t1 * rx;

    const geometry::Point moved  = t1.apply(p1);
    const geometry::Point rotated = rx.apply(p1);
    const geometry::Point combo  = combined.apply(p1);

    std::cout << "\n-- Transforms --\n";
    std::cout << "p1 translated(1,2,3) = " << moved   << "\n";
    std::cout << "p1 rotatedX(45°)     = " << rotated << "\n";
    std::cout << "p1 combined          = " << combo   << "\n";

    // Verify inverse: T * T^-1 should reproduce the original point
    const geometry::Point roundtrip = t1.inverse().apply(moved);
    std::cout << "round-trip (should equal p1): " << roundtrip << "\n";

    // ── Write results to a file ───────────────────────────────────────────────
    log.debug("Writing results to output.txt");
    try {
        io::FileWriter writer("output.txt");
        writer.writeLine("=== Geometry results ===");
        for (const auto& s : shapes) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(4)
                << s->name()
                << "  area=" << s->area()
                << "  perimeter=" << s->perimeter()
                << "  centroid=" << s->centroid().x()
                << "," << s->centroid().y();
            writer.writeLine(oss.str());
        }
        log.info("Results written (" + std::to_string(writer.bytesWritten()) + " bytes)");
    } catch (const std::exception& ex) {
        log.warning(std::string("Could not write output file: ") + ex.what());
    }

    log.info("=== Done ===");
    return 0;
}
