#include <limits>
#include <wb/drawing.h>
#include <wb/utils.h>

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

bool almostCollinear(geometry::Vec2 point1, geometry::Vec2 point2,
                     geometry::Vec2 point3) {
    static const float threshold = 0.0025;
    float totalLength = (point2 - point1).length() + (point3 - point2).length();
    float reducedLength = (point3 - point1).length();
    if ((totalLength - reducedLength) / reducedLength < threshold) {
        return true;
    }
    return false;
}

Drawing Drawing::finalizeDrawing(PartialDrawing &partialDrawing) {
    Drawing result;
    auto points = partialDrawing.getPoints();
    PRINT_DBG(points.size());
    if (points.size() == 1) {
        result.m_points.push_back(points.front());
        result.m_boundingBox.position = points.front() - geometry::Vec2{4, 4};
        result.m_boundingBox.size = geometry::Vec2{9, 9};

    } else {
        geometry::Vec2 lastPoint = points.front();

        int minX = std::numeric_limits<int>::max();
        int minY = std::numeric_limits<int>::max();
        int maxX = std::numeric_limits<int>::min();
        int maxY = std::numeric_limits<int>::min();

        for (std::size_t i = 0; i < points.size(); i++) {
            minX = std::min(minX, points[i].x);
            minY = std::min(minY, points[i].y);
            maxX = std::max(maxX, points[i].x);
            maxY = std::max(maxY, points[i].y);
        }

        result.m_boundingBox.position = geometry::Vec2{minX, minY};
        result.m_boundingBox.size = geometry::Vec2{minX, minY};

        for (std::size_t i = 1; i < points.size(); i++) {
            if (i + 1 < points.size()) {
                if (almostCollinear(lastPoint, points[i], points[i + 1])) {
                    continue;
                }
            }
            result.m_lineSegments.push_back(
                geometry::LineSegment{lastPoint, points[i]});
            lastPoint = points[i];
        }
    }
    PRINT_DBG(result.m_lineSegments.size());
    return result;
}

Drawing Drawing::fromPoints(std::vector<geometry::Vec2> &points) {
    Drawing result;
    if (points.size() == 1) {
        result.m_points.push_back(points.front());
        result.m_boundingBox.position = points.front() - geometry::Vec2{4, 4};
        result.m_boundingBox.size = geometry::Vec2{9, 9};
    } else {

        int minX = std::numeric_limits<int>::max();
        int minY = std::numeric_limits<int>::max();
        int maxX = std::numeric_limits<int>::min();
        int maxY = std::numeric_limits<int>::min();

        for (std::size_t i = 0; i < points.size(); i++) {
            minX = std::min(minX, points[i].x);
            minY = std::min(minY, points[i].y);
            maxX = std::max(maxX, points[i].x);
            maxY = std::max(maxY, points[i].y);
        }

        result.m_boundingBox.position = geometry::Vec2{minX, minY};
        result.m_boundingBox.size = geometry::Vec2{maxX - minX, maxY - minY};

        for (std::size_t i = 1; i < points.size(); i++) {
            result.m_lineSegments.push_back(
                geometry::LineSegment{points[i - 1], points[i]});
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
        if (almostCollinear(line.start, point, line.end)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] bool Drawing::intersects(geometry::Box box) const noexcept {
	if (!m_boundingBox.intersects(box)) {
		return false;
	}
    for (geometry::Vec2 point : m_points) {
        if (box.contains(point)) {
            return true;
        }
    }
    for (geometry::LineSegment line : m_lineSegments) {
        if (box.intersects(line)) {
            return true;
        }
    }
    return false;
}

}; // namespace wb
