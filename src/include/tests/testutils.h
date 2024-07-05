#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

struct TestSetResults {
    int numFailed = 0;
    int numPassed = 0;
};

struct TestResult {
    const char *name;
    bool passed;
    const char *message;
};

#define PASS                                                                   \
    TestResult { .name = name, .passed = true }
#define FAIL(msg)                                                              \
    TestResult { .name = name, .passed = false, .message = msg }

#define RUN_TESTS                                                              \
    for (auto test : tests) {                                                  \
        TestResult result = test();                                            \
        if (result.passed) {                                                   \
            results.numPassed++;                                               \
            std::cout << "Testing '" << result.name << "'...  " << "PASSED\n"; \
        } else {                                                               \
            results.numFailed++;                                               \
            std::cout << "Testing '" << result.name << "'...  "                \
                      << "FAILED: " << result.message << "\n";                 \
        }                                                                      \
    }

#endif
