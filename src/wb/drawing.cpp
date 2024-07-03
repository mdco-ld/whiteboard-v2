#include <wb/drawing.h>

namespace wb {

PartialDrawing::PartialDrawing() {}

void PartialDrawing::addPoint(geometry::Vec2 point) noexcept {
    m_points.push_back(point);
}

[[nodiscard]] const std::vector<geometry::Vec2> &PartialDrawing::getPoints() noexcept {
    return m_points;
}

}; // namespace wb
