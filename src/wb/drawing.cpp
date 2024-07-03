#include "wb/utils.h"
#include <wb/drawing.h>

#include <wb/geometry.h>

namespace wb {

PartialDrawing::PartialDrawing() {}

void PartialDrawing::addPoint(geometry::Vec2 point) noexcept {
    m_points.push_back(point);
}

void PartialDrawing::clear() noexcept { m_points.clear(); }

[[nodiscard]] const std::vector<geometry::Vec2> &
PartialDrawing::getPoints() noexcept {
    return m_points;
}

Drawing Drawing::finalizeDrawing(PartialDrawing &partialDrawing) {
    Drawing result;
    auto points = partialDrawing.getPoints();
    if (points.size() == 1) {
        result.m_points.push_back(points.front());
    } else {
		geometry::Vec2 lastPoint = points.front();
        for (std::size_t i = 1; i < points.size(); i++) {
            if (i + 1 < points.size()) {
                if (geometry::LineSegment{points[i - 1], points[i + 1]}
                        .contains(points[i])) {
					continue;
                }
            }
            result.m_lineSegments.push_back(
                geometry::LineSegment{lastPoint, points[i]});
			lastPoint = points[i];
        }
    }
    return result;
}

[[nodiscard]] const std::vector<geometry::Vec2> &
Drawing::getPoints() const noexcept {
    return m_points;
}

[[nodiscard]] const std::vector<geometry::LineSegment> &
Drawing::getLineSegments() const noexcept {
    return m_lineSegments;
}

[[nodiscard]] bool
Drawing::intersects(geometry::LineSegment line) const noexcept {
    for (geometry::LineSegment segment : m_lineSegments) {
        if (line.intersects(segment)) {
            return true;
        }
    }
    for (geometry::Vec2 point : m_points) {
        if (line.contains(point)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] bool Drawing::intersects(geometry::Box box) const noexcept {
    return true;
}

}; // namespace wb
