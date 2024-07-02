#include "wb/geometry.h"
#include <raylib.h>
#include <wb/utils.h>

int main() {
    geometry::LineSegment line1{geometry::Vec2{0, 10}, geometry::Vec2{10, 0}};
    geometry::LineSegment line2{geometry::Vec2{0, 0}, geometry::Vec2{10, 10}};
	PRINT_DBG(line1.intersects(line2));
    return 0;
}
