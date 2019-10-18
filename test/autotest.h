#pragma once

#include <cstddef>
#define DO_CHECK(expr) check(expr, __FUNCTION__, __FILE__, __LINE__)

class Test {
public:
    static void check(bool expr, const char *func, const char *filename, size_t lineNum);
    virtual void runAllTests() = 0;
};
