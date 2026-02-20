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
#include <sstream>

// ══════════════════════════════════════════════════════════════════════════════
// STUDENT EXERCISE – Dangling Pointer Examples                  ⚠  DO NOT USE
//
// Each function below returns a pointer or reference that becomes invalid the
// moment the function returns, because it points into the stack frame of the
// callee.  Using the result is Undefined Behaviour (UB): the program may crash,
// silently produce wrong output, or appear to work – depending on the compiler,
// optimisation level, and what else is on the stack.
//
// Fix strategies (shown inline as comments):
//   1. Return by value instead of by raw pointer / reference.
//   2. Allocate on the heap (new / make_unique) and transfer ownership.
//   3. Accept the storage as an output parameter owned by the caller.
// ══════════════════════════════════════════════════════════════════════════════

namespace dangling_examples {

// ── Example 1 ─────────────────────────────────────────────────────────────────
// Returns a raw pointer to a local Point.  The Point is destroyed on return;
// the pointer immediately dangles.
//
// FIX: return geometry::Point  (by value)
//   or return std::unique_ptr<geometry::Point>(new geometry::Point(...));
[[deprecated("dangling pointer – for teaching only")]]
static geometry::Point* getLocalPoint() {
    geometry::Point local(1.0, 2.0, 3.0);   // lives on the stack
    return &local;                            // ← stack address escapes!
}                                             //   'local' is destroyed here

// ── Example 2 ─────────────────────────────────────────────────────────────────
// Returns a const reference to a locally-constructed Circle.
// The lifetime of the temporary ends at the closing brace.
//
// FIX: return geometry::Circle  (by value)
[[deprecated("dangling reference – for teaching only")]]
static const geometry::Circle& getLocalCircle() {
    geometry::Circle c(geometry::Point(0.0, 0.0, 0.0), 5.0);  // stack object
    return c;    // ← reference to object about to be destroyed!
}

// ── Example 3 ─────────────────────────────────────────────────────────────────
// Stores the raw address of a local array element, then returns after the
// array goes out of scope.  Classic "pointer to dead stack buffer".
//
// FIX: use std::vector<geometry::Point> and return it by value.
[[deprecated("dangling pointer to stack array – for teaching only")]]
static geometry::Point* getStackArrayElement() {
    geometry::Point pts[3] = {
        geometry::Point(0.0, 0.0, 0.0),
        geometry::Point(1.0, 0.0, 0.0),
        geometry::Point(0.0, 1.0, 0.0),
    };
    return &pts[1];   // ← points into a stack array; invalid after return
}

// ── Demonstration caller ──────────────────────────────────────────────────────
// Calls the three bad functions so students can observe the UB in action
// (e.g. with AddressSanitizer: -fsanitize=address).
// In a release build this may appear to "work" – that is part of the lesson.
static void runDanglingExamples() {
    std::cout << "\n";
    std::cout << "════════════════════════════════════════════════════════\n";
    std::cout << "  EXERCISE: Dangling Pointer Examples  (undefined behaviour)\n";
    std::cout << "════════════════════════════════════════════════════════\n";

    // Example 1 – dangling Point*
    {
        geometry::Point* p = getLocalPoint();   // p is already dangling
        // Dereferencing p is UB.  We print the raw address to show it is a
        // stack address – do NOT use p->x() etc. in real code.
        std::cout << "[Ex 1] dangling Point*  address = " << static_cast<void*>(p)
                  << "  (do not dereference!)\n";
        // ✔ Safe alternative:
        //   geometry::Point safe(1.0, 2.0, 3.0);
        //   std::cout << safe << "\n";
    }

    // Example 2 – dangling Circle&
    {
        const geometry::Circle& ref = getLocalCircle();   // ref is dangling
        // Reading ref.radius() is UB.  We only take its address here.
        std::cout << "[Ex 2] dangling Circle& address = "
                  << static_cast<const void*>(&ref)
                  << "  (do not call any method!)\n";
        // ✔ Safe alternative:
        //   geometry::Circle safe(geometry::Point(0,0,0), 5.0);
        //   std::cout << safe.radius() << "\n";
    }

    // Example 3 – dangling pointer into a stack array
    {
        geometry::Point* p = getStackArrayElement();  // p is already dangling
        std::cout << "[Ex 3] dangling stack-array Point* address = "
                  << static_cast<void*>(p)
                  << "  (do not dereference!)\n";
        // ✔ Safe alternative:
        //   std::vector<geometry::Point> pts = { ... };
        //   geometry::Point* p = &pts[1];   // valid while 'pts' is alive
    }

    std::cout << "════════════════════════════════════════════════════════\n\n";
}

} // namespace dangling_examples

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
    // ── Student exercise: dangling pointers (run first so output is prominent) ──
    dangling_examples::runDanglingExamples();

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
