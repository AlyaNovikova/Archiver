#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cassert>
#include <memory>

const std::size_t size_char = 256, size_tree = 256 * 2 - 1;
typedef std::array<std::size_t, 256> TableOfCounts;

TableOfCounts cnt(std::istream& input);

struct TreeNode {
    std::size_t cnt;
    unsigned char letter;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    TreeNode(std::size_t cnt_, unsigned char letter_);

    TreeNode(std::size_t cnt_, std::shared_ptr<TreeNode> left_, std::shared_ptr<TreeNode> right_);
};


class HuffTree {
public:
    HuffTree(TableOfCounts table);

    std::vector<bool> get_code(unsigned char letter);
    std::shared_ptr<TreeNode> get_root();

private:
    std::array<std::vector<bool>, 256> letter_codes;
    std::shared_ptr<TreeNode> root;

    void build_codes(std::shared_ptr<TreeNode> v, std::vector<bool>& cur_codes);
};


class BinWriter {
public:
    BinWriter(std::ostream&);

    void write(char const* buffer, std::size_t cnt);
    void write_bit(bool bit);
    void flush();

private:
    char cur_byte;
    std::size_t pos;
    std::ostream& stream;
};


class BinReader {
public:
    BinReader(std::istream&);

    void read(char* buffer, std::size_t cnt);
    bool read_bit();

private:
    char cur_byte;
    std::size_t pos;
    std::istream& stream;
};

struct CompressionStatistic {
    std::size_t original;
    std::size_t add_inf;
    std::size_t compressed;
};

std::ostream& operator<< (std::ostream &s, CompressionStatistic data);

CompressionStatistic zip(std::istream& input, std::ostream& output);
CompressionStatistic unzip(std::istream& input, std::ostream& output);
