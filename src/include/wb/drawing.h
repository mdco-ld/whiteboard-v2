#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <vector>
#include <wb/geometry.h>

namespace wb {

class PartialDrawing {
  public:
    PartialDrawing();
    void addPoint(geometry::Vec2) noexcept;
    void clear() noexcept;

    [[nodiscard]] const std::vector<geometry::Vec2> &getPoints() noexcept;

  private:
    std::vector<geometry::Vec2> m_points;
};

class Drawing {
  public:
	static Drawing finalizeDrawing(PartialDrawing &);
	static Drawing fromPoints(std::vector<geometry::Vec2> &);

    [[nodiscard]] const std::vector<geometry::Vec2> &getPoints() const noexcept;
    [[nodiscard]] const std::vector<geometry::LineSegment> &getLineSegments() const noexcept;
	[[nodiscard]] bool intersects(geometry::LineSegment) const noexcept;
	[[nodiscard]] bool intersects(geometry::Box) const noexcept;

  private:
    std::vector<geometry::Vec2> m_points;
    std::vector<geometry::LineSegment> m_lineSegments;
	geometry::Box m_boundingBox;
};


}; // namespace wb

#endif
