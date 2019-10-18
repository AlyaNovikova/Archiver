#include "huffman_test.h"
#include "huffman.h"

#include <sstream>

using namespace std;

void HuffTest::test_cnt_1()
{
    stringstream input;
    input << "abacaba";

    TableOfCounts table = cnt(input);
    DO_CHECK(table['a'] == 4);
    DO_CHECK(table['b'] == 2);
    DO_CHECK(table['c'] == 1);
}

void HuffTest::test_cnt_2()
{
    stringstream input;
    input << "";

    TableOfCounts table = cnt(input);
    DO_CHECK(table['a'] == 0);
    DO_CHECK(table['/'] == 0);
    DO_CHECK(table['$'] == 0);
    DO_CHECK(table['.'] == 0);
}

void HuffTest::test_get_code_1()
{
    stringstream input;
    input << "aaaab";

    TableOfCounts table = cnt(input);
    HuffTree tree(table);

    DO_CHECK(tree.get_code('a').size() == 1);
    DO_CHECK(tree.get_code('b').size() == 2);
}

void HuffTest::test_get_code_2()
{
    stringstream input;
    input << "aaabcd";

    TableOfCounts table = cnt(input);
    HuffTree tree(table);

    DO_CHECK(tree.get_code('a').size() == 1);
    DO_CHECK(tree.get_code('b').size() <= 4);
    DO_CHECK(tree.get_code('c').size() <= 4);
    DO_CHECK(tree.get_code('d').size() <= 4);
    DO_CHECK(tree.get_code('z').size() > 2);
    DO_CHECK(tree.get_code('.').size() > 2);
}

void HuffTest::test_get_root_1() {
    stringstream input;
    input << "aaaaa";

    TableOfCounts table = cnt(input);
    HuffTree tree(table);

    DO_CHECK(tree.get_root()->right->letter == 'a');
    DO_CHECK(tree.get_root()->cnt == 5);
    DO_CHECK(tree.get_root()->right->cnt == 5);
    DO_CHECK(tree.get_root()->left->cnt == 0);
}

void HuffTest::test_get_root_2() {
    stringstream input;
    input << "abacaba";

    TableOfCounts table = cnt(input);
    HuffTree tree(table);

    DO_CHECK(tree.get_root()->right->letter == 'a');
    DO_CHECK(tree.get_root()->cnt == 7);
    DO_CHECK(tree.get_root()->right->cnt == 4);
    DO_CHECK(tree.get_root()->left->cnt == 3);
}

void HuffTest::test_bin_write() {
    stringstream s;
    BinWriter writer(s);
    writer.write("a", 1);

    char c;
    s >> c;
    DO_CHECK(c == 'a');
}

void HuffTest::test_bin_write_bit() {
    stringstream s;
    BinWriter writer(s);

    writer.write_bit(1);
    writer.write_bit(0);
    writer.write_bit(0);
    writer.write_bit(0);
    writer.write_bit(0);
    writer.write_bit(1);
    writer.write_bit(1);
    writer.write_bit(0);

    char c;
    s >> c;
    DO_CHECK(c == 'a');
}

void HuffTest::test_bin_write_flush() {
    stringstream s;
    BinWriter writer(s);

    writer.write_bit(1);
    writer.write_bit(0);
    writer.write_bit(0);
    writer.write_bit(0);
    writer.write_bit(0);
    writer.write_bit(1);
    writer.write_bit(1);
    writer.flush();

    char c;
    s >> c;
    DO_CHECK(c == 'a');
}

void HuffTest::test_bin_read() {
    stringstream s("a");
    BinReader reader(s);

    char c;
    reader.read(&c, 1);
    DO_CHECK(c == 'a');
}

void HuffTest::test_bin_read_bit() {
    stringstream s("a");
    BinReader reader(s);

    DO_CHECK(reader.read_bit() == 1);
    DO_CHECK(reader.read_bit() == 0);
    DO_CHECK(reader.read_bit() == 0);
    DO_CHECK(reader.read_bit() == 0);
    DO_CHECK(reader.read_bit() == 0);
    DO_CHECK(reader.read_bit() == 1);
    DO_CHECK(reader.read_bit() == 1);
    DO_CHECK(reader.read_bit() == 0);
}

void HuffTest::end2end_1()
{
    stringstream origin("abacaba");
    stringstream zipped;
    stringstream unzipped;

    zip(origin, zipped);
    unzip(zipped, unzipped);

    string result;
    unzipped >> result;
    DO_CHECK(result == "abacaba");
}

void HuffTest::end2end_2()
{
    stringstream origin("kokoko");
    stringstream zipped;
    stringstream unzipped;

    CompressionStatistic zip_ = zip(origin, zipped);
    CompressionStatistic unzip_ = unzip(zipped, unzipped);

    string result;
    unzipped >> result;
    DO_CHECK(result == "kokoko");
    DO_CHECK(zip_.original == 6);
    DO_CHECK(unzip_.original == 6);
    DO_CHECK(zip_.add_inf == 2048);
    DO_CHECK(unzip_.add_inf == 2048);
    DO_CHECK(zip_.compressed == unzip_.compressed);
}

void HuffTest::end2end_3()
{
    stringstream origin("");
    stringstream zipped;
    stringstream unzipped;

    CompressionStatistic zip_ = zip(origin, zipped);
    CompressionStatistic unzip_ = unzip(zipped, unzipped);

    string result;
    unzipped >> result;
    DO_CHECK(result == "");
    DO_CHECK(zip_.original == 0);
    DO_CHECK(unzip_.original == 0);
    DO_CHECK(zip_.add_inf == 2048);
    DO_CHECK(unzip_.add_inf == 2048);
    DO_CHECK(zip_.compressed == 0);
    DO_CHECK(unzip_.compressed == 0);
    DO_CHECK(zip_.compressed == unzip_.compressed);
}

void HuffTest::end2end_4()
{
    stringstream origin("111111111");
    stringstream zipped;
    stringstream unzipped;

    CompressionStatistic zip_ = zip(origin, zipped);
    CompressionStatistic unzip_ = unzip(zipped, unzipped);

    string result;
    unzipped >> result;
    DO_CHECK(result == "111111111");
    DO_CHECK(zip_.original == 9);
    DO_CHECK(unzip_.original == 9);
    DO_CHECK(zip_.add_inf == 2048);
    DO_CHECK(unzip_.add_inf == 2048);
    DO_CHECK(zip_.compressed == 2);
    DO_CHECK(unzip_.compressed == 2);
    DO_CHECK(zip_.compressed == unzip_.compressed);
}

void HuffTest::end2end_5()
{
    stringstream origin("1111111");
    stringstream zipped;
    stringstream unzipped;

    CompressionStatistic zip_ = zip(origin, zipped);
    CompressionStatistic unzip_ = unzip(zipped, unzipped);

    string result;
    unzipped >> result;
    DO_CHECK(result == "1111111");
    DO_CHECK(zip_.original == 7);
    DO_CHECK(unzip_.original == 7);
    DO_CHECK(zip_.add_inf == 2048);
    DO_CHECK(unzip_.add_inf == 2048);
    DO_CHECK(zip_.compressed == 1);
    DO_CHECK(unzip_.compressed == 1);
    DO_CHECK(zip_.compressed == unzip_.compressed);
}

void HuffTest::runAllTests()
{
    test_cnt_1();
    test_cnt_2();
    test_get_code_1();
    test_get_code_2();
    test_get_root_1();
    test_get_root_2();
    test_bin_write();
    test_bin_write_bit();
    test_bin_write_flush();
    test_bin_read();
    test_bin_read_bit();
    end2end_1();
    end2end_2();
    end2end_3();
    end2end_4();
    end2end_5();
}
