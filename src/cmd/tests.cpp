#include <iostream>
#include <tests/geometry.h>
#include <tests/serialization.h>
#include <tests/testutils.h>
#include <wb/geometry.h>

int main() {
    auto testSets = {runGeometryTests, runSerializationTests};
    int failed = 0;
    int passed = 0;
    for (auto test : testSets) {
        TestSetResults result = test();
        passed += result.numPassed;
        failed += result.numFailed;
    }
    if (failed == 0) {
        std::cout << "All tests passed! Hooray!\n";
    } else {
        std::cout << "Results: " << failed << "/" << failed + passed
                  << " tests failed\n";
    }
    return 0;
}
