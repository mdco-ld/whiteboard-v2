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
    if (partialDrawing.getPoints().size() == 1) {
        result.m_points.push_back(partialDrawing.getPoints().front());
    } else {
        for (std::size_t i = 1; i < partialDrawing.getPoints().size(); i++) {
            result.m_lineSegments.push_back(
                geometry::LineSegment{partialDrawing.getPoints()[i - 1],
                                      partialDrawing.getPoints()[i]});
        }
    }
    return result;
}

[[nodiscard]] const std::vector<geometry::Vec2> &Drawing::getPoints() const noexcept {
	return m_points;
}

[[nodiscard]] const std::vector<geometry::LineSegment> &Drawing::getLineSegments() const noexcept {
	return m_lineSegments;
}

[[nodiscard]] bool Drawing::intersects(geometry::LineSegment line) const noexcept{
	for (geometry::LineSegment segment : m_lineSegments) {
		if (line.intersects(segment)) {
			return true;
		}
	}
	return false;
}

[[nodiscard]] bool Drawing::intersects(geometry::Box box) const noexcept{
	return true;
}

}; // namespace wb
