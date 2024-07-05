#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <ostream>
#include <vector>

namespace geometry {

struct Vec2 {
    int x;
    int y;
    [[nodiscard]] float length() const noexcept;

    static const Vec2 X;
    static const Vec2 Y;
};

enum class Orientation {
    None,
    Counterclockwise,
    Clockwise,
};

/**
 * Finds the orientation of three points in order
 */
[[nodiscard]] Orientation getOrientation(Vec2, Vec2, Vec2) noexcept;

Vec2 operator+(Vec2, Vec2) noexcept;
Vec2 operator-(Vec2, Vec2) noexcept;
Vec2 operator-(Vec2) noexcept;
Vec2 operator*(Vec2, int) noexcept;
bool operator==(Vec2, Vec2) noexcept;

struct LineSegment {
    Vec2 start;
    Vec2 end;

    [[nodiscard]] bool contains(geometry::Vec2) const noexcept;
    [[nodiscard]] bool intersects(LineSegment) const noexcept;
};

struct Box {
    Vec2 position;
    Vec2 size;

    [[nodiscard]] bool contains(Vec2) const noexcept;
    [[nodiscard]] bool intersects(LineSegment) const noexcept;
    [[nodiscard]] bool intersects(Box) const noexcept;
};

struct Path {
    std::vector<Vec2> points;

    [[nodiscard]] bool intersects(LineSegment) const noexcept;
    [[nodiscard]] bool intersects(Box) const noexcept;
};

std::ostream &operator<<(std::ostream &, Vec2);
std::ostream &operator<<(std::ostream &, Orientation);
std::ostream &operator<<(std::ostream &, LineSegment);
std::ostream &operator<<(std::ostream &, Box);
std::ostream &operator<<(std::ostream &, Path &);

}; // namespace geometry

#endif
