#include "autotest.h"

#include <iostream>

void Test::check(bool expr, const char *func, const char  *filename, std::size_t lineNum)
{
    if (!expr)
    {
        std::cout << "test failed: " << func << " in " << filename << ":" << lineNum << std::endl;
    }
}
