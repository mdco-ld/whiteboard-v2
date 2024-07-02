#include <wb/geometry.h>

#include <cmath>

namespace geometry {

const Vec2 Vec2::X = Vec2{.x = 1, .y = 0};
const Vec2 Vec2::Y = Vec2{.x = 0, .y = 1};

[[nodiscard]] float Vec2::length() const noexcept {
    return std::sqrt(x * x + y * y);
}

[[nodiscard]] int shoelace(Vec2 point1, Vec2 point2) noexcept {
    return point1.x * point2.y - point1.y * point2.x;
}

[[nodiscard]] Orientation getOrientation(Vec2 point1, Vec2 point2,
                                         Vec2 point3) noexcept {
    int area = shoelace(point1, point2) + shoelace(point2, point3) +
               shoelace(point3, point1);
    if (area == 0) {
        return Orientation::None;
    }
    if (area > 0) {
        return Orientation::Counterclockwise;
    }
    return Orientation::Counterclockwise;
}

Vec2 operator+(Vec2 point1, Vec2 point2) noexcept {
    return Vec2{.x = point1.x + point2.x, .y = point1.y + point2.y};
}

Vec2 operator-(Vec2 point1, Vec2 point2) noexcept {
    return Vec2{.x = point1.x - point2.x, .y = point1.y - point2.y};
}

Vec2 operator-(Vec2 point1) noexcept {
    return Vec2{.x = -point1.x, .y = -point1.y};
}

Vec2 operator*(Vec2 point1, int scale) noexcept {
    return Vec2{.x = -point1.x, .y = -point1.y};
}

[[nodiscard]] bool LineSegment::intersects(LineSegment other) const noexcept {
    if (getOrientation(start, end, other.start) ==
        getOrientation(start, end, other.end)) {
        return false;
    }
    if (getOrientation(other.start, other.end, start) ==
        getOrientation(other.start, other.end, end)) {
        return false;
    }
    return true;
}

[[nodiscard]] bool Box::contains(Vec2 point) const noexcept {
    return position.x <= point.x && point.x <= position.x + size.x &&
           position.y <= point.y && point.y <= position.y + size.y;
}

[[nodiscard]] bool Box::intersects(LineSegment line) const noexcept {
    if (line.intersects(LineSegment{.start = position,
                                    .end = position + Vec2::X * size.x})) {
        return true;
    }
    if (line.intersects(LineSegment{.start = position,
                                    .end = position + Vec2::Y * size.y})) {
        return true;
    }
    if (line.intersects(LineSegment{.start = position + size,
                                    .end = position + Vec2::X * size.x})) {
        return true;
    }
    if (line.intersects(LineSegment{.start = position + size,
                                    .end = position + Vec2::Y * size.y})) {
        return true;
    }
    return false;
}

}; // namespace geometry
