#include <tests/geometry.h>
#include <tests/testutils.h>

#include <iostream>
#include <wb/geometry.h>

namespace geometryTests {

TestResult test1() {
    const char *name = "Lines should intersect";
    geometry::LineSegment line1{geometry::Vec2{0, 10}, geometry::Vec2{10, 0}};
    geometry::LineSegment line2{geometry::Vec2{0, 0}, geometry::Vec2{10, 10}};
    if (line1.intersects(line2)) {
        return PASS;
    }
    return FAIL("Lines do not intersect");
}

TestResult test2() {
    const char *name = "Lines should not intersect";
    geometry::LineSegment line1{geometry::Vec2{0, 0}, geometry::Vec2{10, 0}};
    geometry::LineSegment line2{geometry::Vec2{10, 0}, geometry::Vec2{10, 10}};
    if (line1.intersects(line2)) {
        return PASS;
    }
    return FAIL("Lines intersect");
}

TestResult test3() {
    const char *name = "Boxes should intersect 1";
    geometry::Box box1{geometry::Vec2{0, 0}, geometry::Vec2{10, 10}};
    geometry::Box box2{geometry::Vec2{5, 5}, geometry::Vec2{10, 10}};
    if (box1.intersects(box2)) {
        return PASS;
    }
    return FAIL("Boxes do not intersect");
}

TestResult test4() {
    const char *name = "Boxes should intersect 2";
    geometry::Box box1{geometry::Vec2{-100, -100}, geometry::Vec2{10, 10}};
    geometry::Box box2{geometry::Vec2{-105, -105}, geometry::Vec2{10, 10}};
    if (box1.intersects(box2)) {
        return PASS;
    }
    return FAIL("Boxes do not intersect");
}

TestResult test5() {
    const char *name = "Box should intersect line 1";
    geometry::Box box1{geometry::Vec2{-100, -100}, geometry::Vec2{10, 10}};
    geometry::LineSegment line{geometry::Vec2{-95, -95},
                               geometry::Vec2{-50, -10}};
    if (box1.intersects(line)) {
        return PASS;
    }
    return FAIL("Box does not intersect line");
}

TestResult test6() {
    const char *name = "Box should intersect line 2";
    geometry::Box box1{geometry::Vec2{-100, -100}, geometry::Vec2{10, 10}};
    geometry::LineSegment line{geometry::Vec2{-95, -95},
                               geometry::Vec2{-93, -93}};
    if (box1.intersects(line)) {
        return PASS;
    }
    return FAIL("Box does not intersect line");
}

}; // namespace geometryTests

TestSetResults runGeometryTests() {
    TestSetResults results;
    using namespace geometryTests;
    auto tests = {test1, test2, test3, test4, test5, test6};
    std::cout << "Running geometry tests...\n";
	RUN_TESTS;
    return results;
}
