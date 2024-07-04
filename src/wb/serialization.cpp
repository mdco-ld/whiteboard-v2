#include <istream>
#include <ostream>
#include <vector>
#include <wb/serialization.h>

namespace serialization {

void serialize(std::ostream &out, std::vector<wb::Drawing> &drawings) {
    out << drawings.size() << "\n";
    for (wb::Drawing &drawing : drawings) {
        if (drawing.getPoints().size() > 0) {
            out << 1 << "\n";
            out << drawing.getPoints().front().x << " "
               << drawing.getPoints().front().y << "\n";
            continue;
        }
        out << drawing.getLineSegments().size() + 1 << "\n";
        out << drawing.getLineSegments().front().start.x << " "
           << drawing.getLineSegments().front().start.y << " ";
        for (geometry::LineSegment line : drawing.getLineSegments()) {
            out << line.end.x << " " << line.end.y << " ";
        }
        out << "\n";
    }
}

std::vector<wb::Drawing> deserialize(std::istream &in) {
    std::vector<wb::Drawing> result;
    int numDrawings;
    in >> numDrawings;
    for (int _ = 0; _ < numDrawings; _++) {
        std::vector<geometry::Vec2> points;
        int numPoints;
        in >> numPoints;
        for (int i = 0; i < numPoints; i++) {
            int x;
            int y;
            in >> x >> y;
            points.push_back(geometry::Vec2{x, y});
        }
        result.push_back(wb::Drawing::fromPoints(points));
    }
    return result;
}

}; // namespace serialization
