#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

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

struct LineSegment {
    Vec2 start;
    Vec2 end;

    [[nodiscard]] bool intersects(LineSegment) const noexcept;
};

struct Box {
    Vec2 position;
    Vec2 size;

    [[nodiscard]] bool contains(Vec2) const noexcept;
    [[nodiscard]] bool intersects(LineSegment) const noexcept;
    [[nodiscard]] bool intersects(Box) const noexcept;
};

}; // namespace geometry

#endif
