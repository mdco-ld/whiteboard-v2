#include <wb/rendering.h>

#include <wb/drawing.h>
#include <wb/geometry.h>
#include <wb/view.h>

#include <raylib.h>

namespace rendering {

Vector2 tovec(geometry::Vec2 point) {
    return Vector2{.x = (float)point.x, .y = (float)point.y};
}

void render(wb::View &view, geometry::Vec2 point) {
    geometry::Vec2 screenPosition = view.getScreenPosition(point);
    DrawCircle(screenPosition.x, screenPosition.y, view.getZoom() * 2.0f,
               BLACK);
}

void render(wb::View &view, geometry::LineSegment line) {
    render(view, line.start);
    render(view, line.end);
    DrawLineEx(tovec(view.getScreenPosition(line.start)),
               tovec(view.getScreenPosition(line.end)), 4.0f * view.getZoom(),
               BLACK);
}

void render(wb::View &view, wb::PartialDrawing &drawing) {
    auto points = drawing.getPoints();
    for (std::size_t i = 1; i < points.size(); i++) {
        render(view, geometry::LineSegment{points[i - 1], points[i]});
    }
}

void render(wb::View &view, wb::Drawing &drawing) {
    for (geometry::Vec2 point : drawing.getPoints()) {
        render(view, point);
    }
    for (geometry::LineSegment line : drawing.getLineSegments()) {
        render(view, line);
    }
}

}; // namespace rendering
