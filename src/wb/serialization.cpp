#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <wb/geometry.h>
#include <wb/serialization.h>

namespace serialization {

void serializeV1(std::ostream &out, int &value) {
    out.write(reinterpret_cast<const char *>(&value), sizeof(int));
}

void deserializeV1(std::istream &in, int &output) {
    in.read(reinterpret_cast<char *>(&output), sizeof(int));
}

void serializeV1(std::ostream &out, geometry::Vec2 &value) {
    serializeV1(out, value.x);
    serializeV1(out, value.y);
}

void deserializeV1(std::istream &in, geometry::Vec2 &output) {
    deserializeV1(in, output.x);
    deserializeV1(in, output.y);
}

void serializeV1(std::ostream &out, geometry::LineSegment &value) {
    serializeV1(out, value.start);
    serializeV1(out, value.end);
}

void deserializeV1(std::istream &in, geometry::LineSegment &output) {
    deserializeV1(in, output.start);
    deserializeV1(in, output.end);
}

void serializeV1(std::ostream &out, geometry::Box &value) {
    serializeV1(out, value.position);
    serializeV1(out, value.size);
}

void deserializeV1(std::istream &in, geometry::Box &output) {
    deserializeV1(in, output.position);
    deserializeV1(in, output.size);
}

void serializeHeader(std::ostream &out, int8_t majorVersion,
                     int8_t minorVersion) {
    const char header[] = {'W', 'B', majorVersion, minorVersion};
    out.write(header, 4);
}

void deserializeHeader(std::istream &in, int &outputMajorVersion,
                       int &outputMinorVersion) {
    char header[5];
    in.read(header, 4);
    if (header[0] != 'W' || header[1] != 'B') {
        throw std::runtime_error("Invalid magic bytes for deserialization");
    }
    outputMajorVersion = header[3];
    outputMinorVersion = header[4];
}

template <> void serialize<int>(std::ostream &out, int &value) {
    serializeV1(out, value);
}

template <> void deserialize<int>(std::istream &in, int &output) {
    deserializeV1(in, output);
}

template <>
void serialize<geometry::Vec2>(std::ostream &out, geometry::Vec2 &value) {
    serializeV1(out, value);
}

template <>
void deserialize<geometry::Vec2>(std::istream &in, geometry::Vec2 &output) {
    deserializeV1(in, output);
}

template <>
void serialize<geometry::LineSegment>(std::ostream &out,
                                      geometry::LineSegment &value) {
    serializeV1(out, value);
}

template <>
void deserialize<geometry::LineSegment>(std::istream &in,
                                        geometry::LineSegment &output) {
    deserializeV1(in, output);
}

template <>
void serialize<geometry::Box>(std::ostream &out, geometry::Box &value) {
    serializeV1(out, value);
}

template <>
void deserialize<geometry::Box>(std::istream &in, geometry::Box &output) {
    deserializeV1(in, output);
}

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
