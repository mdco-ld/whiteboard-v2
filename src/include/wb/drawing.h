#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <vector>
#include <wb/geometry.h>

namespace wb {

struct Drawing {
	std::vector<geometry::Vec2> points;
	std::vector<geometry::LineSegment> lineSegments;
};

class PartialDrawing {
  public:
    PartialDrawing();
    void addPoint(geometry::Vec2) noexcept;
    void clear() noexcept;

    [[nodiscard]] const std::vector<geometry::Vec2> &getPoints() noexcept;

  private:
    std::vector<geometry::Vec2> m_points;
};

Drawing finalizeDrawing(PartialDrawing &);

}; // namespace wb

#endif
