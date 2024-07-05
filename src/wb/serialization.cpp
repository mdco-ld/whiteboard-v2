#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <wb/drawing.h>
#include <wb/geometry.h>
#include <wb/serialization.h>
#include <wb/utils.h>

namespace serialization {

void serializeV1(std::ostream &out, int &value) {
    unsigned int v = value;
    unsigned char w[4];
    w[0] = (v & 0xff000000) >> 24;
    w[1] = (v & 0xff0000) >> 16;
    w[2] = (v & 0xff00) >> 8;
    w[3] = v & 0xff;
    out.write(reinterpret_cast<const char *>(w), 4);
}

void deserializeV1(std::istream &in, int &output) {
    unsigned int v = 0;
    unsigned char w[4];
    in.read(reinterpret_cast<char *>(w), 4);
    v += w[0] << 24;
    v += w[1] << 16;
    v += w[2] << 8;
    v += w[3];
    output = v;
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

void serializeV1(std::ostream &, wb::Drawing &);
void deserializeV1(std::istream &, wb::Drawing &);

template <typename T>
void serializeVectorV1(std::ostream &out, std::vector<T> &values) {
    int length = values.size();
    serializeV1(out, length);
    for (T &value : values) {
        serializeV1(out, value);
    }
}

template <typename T>
void deserializeVectorV1(std::istream &in, std::vector<T> &values) {
    if (!values.empty()) {
        values.clear();
    }
    int length;
    deserializeV1(in, length);
    if (length < 0) {
        throw std::runtime_error("Invalid deserialized length");
    }
    values.resize(length);
    for (T &value : values) {
        deserializeV1(in, value);
    }
}

void serializeV1(std::ostream &out, wb::Drawing &drawing) {
    std::vector<geometry::Vec2> points = drawing.getPoints();
    serializeVectorV1(out, points);
    std::vector<geometry::LineSegment> lines = drawing.getLineSegments();
    serializeVectorV1(out, lines);
}

void deserializeV1(std::istream &in, wb::Drawing &drawing) {
    std::vector<geometry::Vec2> points;
    deserializeVectorV1(in, points);
    std::vector<geometry::LineSegment> lines;
    deserializeVectorV1(in, lines);
    if (!points.empty()) {
        drawing = wb::Drawing::fromPoints(points);
        return;
    }
    points.push_back(lines.front().start);
    for (geometry::LineSegment line : lines) {
        points.push_back(line.end);
    }
    drawing = wb::Drawing::fromPoints(points);
}

void serializeV1(std::ostream &out, std::vector<wb::Drawing> &drawings) {
    serializeVectorV1(out, drawings);
}

void deserializeV1(std::istream &in, std::vector<wb::Drawing> &drawings) {
    deserializeVectorV1(in, drawings);
}

void serializeHeader(std::ostream &out, int32_t version) {
    const char header[] = {'W', 'B'};
    out.write(header, 2);
    serializeV1(out, version);
}

void deserializeHeader(std::istream &in, int &version) {
    char header[2];
    in.read(header, 2);
    if (header[0] != 'W' || header[1] != 'B') {
        throw std::runtime_error("Invalid magic bytes for deserialization");
    }
	deserializeV1(in, version);
}

void serialize(std::ostream &out, std::vector<wb::Drawing> &drawings) {
    // out << drawings.size() << "\n";
    // for (wb::Drawing &drawing : drawings) {
    //     if (drawing.getPoints().size() > 0) {
    //         out << 1 << "\n";
    //         out << drawing.getPoints().front().x << " "
    //             << drawing.getPoints().front().y << "\n";
    //         continue;
    //     }
    //     out << drawing.getLineSegments().size() + 1 << "\n";
    //     out << drawing.getLineSegments().front().start.x << " "
    //         << drawing.getLineSegments().front().start.y << " ";
    //     for (geometry::LineSegment line : drawing.getLineSegments()) {
    //         out << line.end.x << " " << line.end.y << " ";
    //     }
    //     out << "\n";
    // }
    serializeHeader(out, 1);
    serializeV1(out, drawings);
}

void deserialize(std::istream &in, std::vector<wb::Drawing> &output) {
    // if (!output.empty()) {
    //     output.clear();
    // }
    // int numDrawings;
    // in >> numDrawings;
    // if (numDrawings < 0) {
    //     throw std::runtime_error("Invalid number of drawings");
    // }
    // for (int _ = 0; _ < numDrawings; _++) {
    //     std::vector<geometry::Vec2> points;
    //     int numPoints;
    //     in >> numPoints;
    //     for (int i = 0; i < numPoints; i++) {
    //         int x;
    //         int y;
    //         in >> x >> y;
    //         points.push_back(geometry::Vec2{x, y});
    //     }
    //     output.push_back(wb::Drawing::fromPoints(points));
    // }
	int version;
	deserializeHeader(in, version);
	if (version == 1) {
		deserializeV1(in, output);
	} else {
		throw std::runtime_error("Unsupported format version");
	}
	PRINT_DBG(output.size());
}

}; // namespace serialization
