#include <wb/drawing.h>

namespace wb {

PartialDrawing::PartialDrawing() {}

void PartialDrawing::addPoint(geometry::Vec2 point) noexcept {
    m_points.push_back(point);
}

void PartialDrawing::clear() noexcept {
	m_points.clear();
}

[[nodiscard]] const std::vector<geometry::Vec2> &PartialDrawing::getPoints() noexcept {
    return m_points;
}

}; // namespace wb
