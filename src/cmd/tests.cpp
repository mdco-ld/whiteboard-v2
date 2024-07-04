#include <iostream>
#include <wb/geometry.h>

struct Result {
    bool success;
    const char *message;
    const char *name;
};

void print(Result res) {
    std::cout << "Test '" << res.name << "' -> ";
    if (res.success) {
        std::cout << "Success!";
    } else {
        std::cout << "Failed: " << res.message;
    }
    std::cout << std::endl;
}

Result test1() {
    const char *name = "Lines should intersect";
    geometry::LineSegment line1{geometry::Vec2{0, 10}, geometry::Vec2{10, 0}};
    geometry::LineSegment line2{geometry::Vec2{0, 0}, geometry::Vec2{10, 10}};
    if (line1.intersects(line2)) {
        return Result{
            .success = true,
            .name = name,
        };
    }
    return Result{
        .success = false, .message = "Lines do not intersect", .name = name};
}

Result test2() {
    const char *name = "Lines should not intersect";
    geometry::LineSegment line1{geometry::Vec2{0, 0}, geometry::Vec2{10, 0}};
    geometry::LineSegment line2{geometry::Vec2{10, 0}, geometry::Vec2{10, 10}};
    if (line1.intersects(line2)) {
        return Result{
            .success = true,
            .name = name,
        };
    }
    return Result{.success = false, .message = "Lines intersect", .name = name};
}

Result test3() {
    const char *name = "Boxes should intersect 1";
    geometry::Box box1{geometry::Vec2{0, 0}, geometry::Vec2{10, 10}};
    geometry::Box box2{geometry::Vec2{5, 5}, geometry::Vec2{10, 10}};
    if (box1.intersects(box2)) {
        return Result{
            .success = true,
            .name = name,
        };
    }
    return Result{
        .success = false, .message = "Boxes do not intersect", .name = name};
}

Result test4() {
    const char *name = "Boxes should intersect 2";
    geometry::Box box1{geometry::Vec2{-100, -100}, geometry::Vec2{10, 10}};
    geometry::Box box2{geometry::Vec2{-105, -105}, geometry::Vec2{10, 10}};
    if (box1.intersects(box2)) {
        return Result{
            .success = true,
            .name = name,
        };
    }
    return Result{
        .success = false, .message = "Boxes do not intersect", .name = name};
}

Result test5() {
    const char *name = "Box should intersect line 1";
    geometry::Box box1{geometry::Vec2{-100, -100}, geometry::Vec2{10, 10}};
    geometry::LineSegment line{geometry::Vec2{-95, -95}, geometry::Vec2{-50, -10}};
    if (box1.intersects(line)) {
        return Result{
            .success = true,
            .name = name,
        };
    }
    return Result{.success = false,
                  .message = "Box does not intersect line",
                  .name = name};
}

Result test6() {
    const char *name = "Box should intersect line 2";
    geometry::Box box1{geometry::Vec2{-100, -100}, geometry::Vec2{10, 10}};
    geometry::LineSegment line{geometry::Vec2{-95, -95}, geometry::Vec2{-93, -93}};
    if (box1.intersects(line)) {
        return Result{
            .success = true,
            .name = name,
        };
    }
    return Result{.success = false,
                  .message = "Box does not intersect line",
                  .name = name};
}

int main() {
    auto tests = {test1, test2, test3, test4, test5, test6};
    int failed = 0;
    int success = 0;
    for (auto test : tests) {
        Result result = test();
        print(result);
        if (result.success) {
            success++;
        } else {
            failed++;
        }
    }
    std::cout << "Results: " << success << " successful " << failed << " failed"
              << std::endl;
    return 0;
}
