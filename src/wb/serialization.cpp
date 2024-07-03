#include <sstream>
#include <vector>
#include <wb/serialization.h>

namespace serialization {

std::string serialize(std::vector<wb::Drawing> &drawings) {
    std::stringstream ss;
    ss << drawings.size() << "\n";
    for (wb::Drawing &drawing : drawings) {
        if (drawing.getPoints().size() > 0) {
            ss << 1 << "\n";
            ss << drawing.getPoints().front().x << " "
               << drawing.getPoints().front().y << "\n";
            continue;
        }
        ss << drawing.getLineSegments().size() + 1 << "\n";
        ss << drawing.getLineSegments().front().start.x << " "
           << drawing.getLineSegments().front().start.y << " ";
        for (geometry::LineSegment line : drawing.getLineSegments()) {
            ss << line.end.x << " " << line.end.y << " ";
        }
        ss << "\n";
    }
    return ss.str();
}

std::vector<wb::Drawing> deserialize(std::string data) {
    std::stringstream ss;
    ss << data;
    std::vector<wb::Drawing> result;
    int numDrawings;
    ss >> numDrawings;
    for (int _ = 0; _ < numDrawings; _++) {
        std::vector<geometry::Vec2> points;
        int numPoints;
        ss >> numPoints;
        for (int i = 0; i < numPoints; i++) {
            int x;
            int y;
            ss >> x >> y;
            points.push_back(geometry::Vec2{x, y});
        }
        result.push_back(wb::Drawing::fromPoints(points));
    }
    return result;
}

}; // namespace serialization
