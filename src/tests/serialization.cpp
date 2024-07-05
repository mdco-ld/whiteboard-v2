#include <iostream>
#include <sstream>
#include <tests/serialization.h>
#include <tests/testutils.h>
#include <wb/geometry.h>
#include <wb/serialization.h>
#include <wb/utils.h>

namespace serialization {

void serializeV1(std::ostream &, int &);
void deserializeV1(std::istream &, int &);
void serializeV1(std::ostream &, geometry::Vec2 &);
void deserializeV1(std::istream &, geometry::Vec2 &);
void serializeV1(std::ostream &, geometry::LineSegment &);
void deserializeV1(std::istream &, geometry::LineSegment &);
template <typename T> void serializeVectorV1(std::ostream &, std::vector<T> &);
template <typename T>
void deserializeVectorV1(std::istream &, std::vector<T> &);

}; // namespace serialization

namespace serializationTests {

TestResult test1() {
    const char *name = "V1: Serializing an int 1";
    std::stringstream ss;
    int value = 157;
    serialization::serializeV1(ss, value);
    int value2;
    serialization::deserializeV1(ss, value2);
    if (value == value2) {
        return PASS;
    }
    return FAIL("Output does not match original number");
}

TestResult test1_2() {
    const char *name = "V1: Serializing an int 2";
    std::stringstream ss;
    int value = 256;
    serialization::serializeV1(ss, value);
    int value2;
    serialization::deserializeV1(ss, value2);
    if (value == value2) {
        return PASS;
    }
    return FAIL("Output does not match original number");
}

TestResult test1_3() {
    const char *name = "V1: Serializing an int 3";
    std::stringstream ss;
    int value = -40145;
    serialization::serializeV1(ss, value);
    int value2;
    serialization::deserializeV1(ss, value2);
    if (value == value2) {
        return PASS;
    }
    return FAIL("Output does not match original number");
}

TestResult test2() {
    const char *name = "V1: Serializing a point 1";
    std::stringstream ss;
    geometry::Vec2 point{41542, 39243};
    serialization::serializeV1(ss, point);
    geometry::Vec2 output;
    serialization::deserializeV1(ss, output);
    if (point == output) {
        return PASS;
    }
    return FAIL("Output does not equal original point");
}

TestResult test3() {
    const char *name = "V1: Serializing a line 1";
    std::stringstream ss;
    geometry::LineSegment line{geometry::Vec2{123, 4523},
                               geometry::Vec2{8510, 5829}};
    serialization::serializeV1(ss, line);
    geometry::LineSegment output;
    serialization::deserializeV1(ss, output);
    if (line.start == output.start && line.end == output.end) {
        return PASS;
    }
    return FAIL("Output does not equal original line");
}

TestResult test4() {
    const char *name = "V1: Serializing a vector<geometry::Vec2> 1";
    std::stringstream ss;
    std::vector<geometry::Vec2> v = {{396, 2}, {352634, 82}, {543, 8234}};
    serialization::serializeVectorV1(ss, v);
    std::vector<geometry::Vec2> output;
    serialization::deserializeVectorV1(ss, output);
    if (v.size() != output.size()) {
        return FAIL("Output does not match original size");
    }
    for (std::size_t i = 0; i < v.size(); i++) {
        if (!(v[i] == output[i])) {
            return FAIL("Output does not equal original vector");
        }
    }
    return PASS;
}

}; // namespace serializationTests

TestSetResults runSerializationTests() {
    TestSetResults results;
    using namespace serializationTests;
    auto tests = {test1, test1_2, test1_3, test2, test3, test4};
    std::cout << "Running serialization tests...\n";
    RUN_TESTS;
    return results;
}
