#include <ostream>
#include <wb/geometry.h>

#include <wb/utils.h>

#include <cmath>

namespace geometry {

const Vec2 Vec2::X = Vec2{.x = 1, .y = 0};
const Vec2 Vec2::Y = Vec2{.x = 0, .y = 1};

[[nodiscard]] float Vec2::length() const noexcept {
    return std::sqrt(x * x + y * y);
}

[[nodiscard]] inline int shoelace(Vec2 point1, Vec2 point2) noexcept {
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
    return Orientation::Clockwise;
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
    return Vec2{.x = point1.x * scale, .y = point1.y * scale};
}

bool operator==(Vec2 point1, Vec2 point2) noexcept {
    return point1.x == point2.x && point1.y == point2.y;
}

[[nodiscard]] bool LineSegment::contains(Vec2 point) const noexcept {
    return getOrientation(start, end, point) == Orientation::None;
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
    if (contains(line.start) || contains(line.end)) {
        return true;
    }
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

[[nodiscard]] bool Box::intersects(Box other) const noexcept {
    if (contains(other.position) || contains(other.position + other.size)) {
        return true;
    }
    if (contains(other.position + Vec2::X * other.size.x) ||
        contains(other.position + Vec2::Y * other.size.y)) {
        return true;
    }
    if (other.contains(position) || other.contains(position + size)) {
        return true;
    }
    if (other.contains(position + Vec2::X * size.x) ||
        other.contains(position + Vec2::Y * size.y)) {
        return true;
    }
    if (other.intersects(LineSegment{.start = position,
                                     .end = position + Vec2::X * size.x})) {
        return true;
    }
    if (other.intersects(LineSegment{.start = position,
                                     .end = position + Vec2::Y * size.y})) {
        return true;
    }
    if (other.intersects(LineSegment{.start = position + size,
                                     .end = position + Vec2::X * size.x})) {
        return true;
    }
    if (other.intersects(LineSegment{.start = position + size,
                                     .end = position + Vec2::Y * size.y})) {
        return true;
    }
    return false;
}

[[nodiscard]] bool Path::intersects(LineSegment line) const noexcept {
    if (points.empty()) {
        return false;
    }
    if (points.size() == 1) {
        return line.contains(points.front());
    }
    for (std::size_t i = 1; i < points.size(); i++) {
        if (line.intersects(LineSegment{points[i - 1], points[i]})) {
            return true;
        }
    }
    return false;
}

std::ostream &operator<<(std::ostream &out, Vec2 point) {
    return out << "Vec2(" << point.x << ", " << point.y << ")";
}

std::ostream &operator<<(std::ostream &out, Orientation orientation) {
    switch (orientation) {
    case Orientation::None:
        return out << "Orientation::None";
    case Orientation::Counterclockwise:
        return out << "Orientation::Counterclockwise";
    case Orientation::Clockwise:
        return out << "Orientation::Clockwise";
    }
    return out << "Orientation::Invalid";
}

std::ostream &operator<<(std::ostream &out, LineSegment line) {
    return out << "Line[start = (" << line.start.x << ", " << line.start.y
               << "), end = (" << line.end.x << ", " << line.end.y << ")]";
}

std::ostream &operator<<(std::ostream &out, Box box) {
    return out << "Box[position = " << box.position << ", size = " << box.size
               << "]";
}

std::ostream &operator<<(std::ostream &out, Path &path) {
    if (path.points.empty()) {
        return out << "Path[]";
    }
    out << "Path[" << path.points.front();
    for (std::size_t i = 1; i < path.points.size(); i++) {
        out << ", " << path.points[i];
    }
    return out << "]";
}

}; // namespace geometry
