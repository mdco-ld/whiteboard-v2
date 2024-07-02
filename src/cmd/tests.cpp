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
    geometry::LineSegment line1{geometry::Vec2{0, 10}, geometry::Vec2{10, 0}};
    geometry::LineSegment line2{geometry::Vec2{0, 0}, geometry::Vec2{10, 10}};
    if (line1.intersects(line2)) {
        return Result{
            .success = true,
            .name = "Lines should intersect",
        };
    }
    return Result{.success = false,
                  .message = "Lines do not intersect",
                  .name = "Lines should intersect"};
}

int main() {
    auto tests = {test1};
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
