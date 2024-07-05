#include <iostream>
#include <sstream>
#include <tests/serialization.h>
#include <tests/testutils.h>
#include <wb/geometry.h>
#include <wb/serialization.h>

namespace serializationTests {

TestResult test1() {
    const char *name = "Serializing an int 1";
    std::stringstream ss;
    int value = 157;
    serialization::serialize(ss, value);
    int value2;
    serialization::deserialize(ss, value2);
    if (value == value2) {
        return PASS;
    }
    return FAIL("Failed to serialize and deserialize an int");
}

TestResult test2() {
    const char *name = "Serializing a point";
    std::stringstream ss;
    geometry::Vec2 point{41542, 39243};
    serialization::serialize(ss, point);
    geometry::Vec2 output;
    serialization::deserialize(ss, output);
    if (point == output) {
        return PASS;
    }
    return FAIL("Failed to serialize and deserialize a point");
}

TestResult test3() {
    const char *name = "Serializing a vector<int>";
    std::stringstream ss;
    std::vector<int> v = {413, 23, 41, 4, 234, 94, 653};
    std::vector<int> output;
    serialization::serialize(ss, v);
    serialization::deserialize(ss, output);
    if (v.size() != output.size()) {
        return FAIL("Output does not match original size");
    }
    for (std::size_t i = 0; i < v.size(); i++) {
        if (v[i] != output[i]) {
            return FAIL("output does not match original vector");
        }
    }
    return PASS;
}

}; // namespace serializationTests

TestSetResults runSerializationTests() {
    TestSetResults results;
    using namespace serializationTests;
    auto tests = {test1, test2, test3};
    std::cout << "Running serialization tests...\n";
    RUN_TESTS;
    return results;
}
