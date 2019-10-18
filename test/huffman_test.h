#include "autotest.h"

class HuffTest: public Test {
public:
    void test_cnt_1();
    void test_cnt_2();
    void test_get_code_1();
    void test_get_code_2();
    void test_get_root_1();
    void test_get_root_2();
    void test_bin_write();
    void test_bin_write_bit();
    void test_bin_write_flush();
    void test_bin_read();
    void test_bin_read_bit();
    void end2end_1();
    void end2end_2();
    void end2end_3();
    void end2end_4();
    void end2end_5();

    void runAllTests();
};
