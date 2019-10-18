#include "huffman.h"

#include <algorithm>
#include <set>

using namespace std;

TableOfCounts cnt(istream& in)
{
    TableOfCounts table = {};
    unsigned char c;
    while (in.read((char*)&c, 1))
    {
        table[c]++;
    }
    return table;
}

TreeNode::TreeNode(size_t cnt_, unsigned char letter_)
{
    cnt = cnt_;
    letter = letter_;
    left = nullptr;
    right = nullptr;
}

TreeNode::TreeNode(size_t cnt_, shared_ptr<TreeNode> left_, shared_ptr<TreeNode> right_)
{
    cnt = cnt_;
    left = left_;
    right = right_;
}

void HuffTree::build_codes(shared_ptr<TreeNode> v, vector<bool>& cur_codes)
{
    if (v->left == nullptr)
    {
        letter_codes[v->letter] = cur_codes;
        return;
    }

    cur_codes.push_back(0);
    build_codes(v->left, cur_codes);

    cur_codes[cur_codes.size() - 1] = 1;
    build_codes(v->right, cur_codes);

    cur_codes.pop_back();
}

struct Comparator {
    bool operator()(shared_ptr<TreeNode> const& t1, shared_ptr<TreeNode> const& t2) {
        return (t1->cnt < t2->cnt);
    }
};

HuffTree::HuffTree(TableOfCounts table)
{
    multiset<shared_ptr<TreeNode>, Comparator> nodes;

    generate_n(inserter(nodes, nodes.end()), 256, [i = -1, &table] () mutable {
        i++;
        return make_shared<TreeNode>(table[i], char(i));
    });

    while(nodes.size() > 1)
    {
        auto min1 = *nodes.begin();
        nodes.erase(nodes.begin());

        auto min2 = *nodes.begin();
        nodes.erase(nodes.begin());

        nodes.insert(make_shared<TreeNode>(min1->cnt + min2->cnt, min1, min2));
    }

    root = *nodes.begin();

    vector<bool> cur_codes;
    build_codes(root, cur_codes);
}

vector<bool> HuffTree::get_code(unsigned char letter)
{
    return letter_codes[letter];
}

shared_ptr<TreeNode> HuffTree::get_root()
{
    return root;
}

BinWriter::BinWriter(ostream& stream) : cur_byte(0), pos(0), stream(stream) {}

void BinWriter::write(char const* buffer, size_t cnt)
{
    assert (pos == 0);
    stream.write(buffer, cnt);
}

void BinWriter::write_bit(bool bit) {
    cur_byte ^= (bit << pos);
    pos++;
    if (pos == 8) {
        flush();
    }
}

void BinWriter::flush()
{
    if (pos != 0)
    {
        pos = 0;
        write(&cur_byte, 1);
        cur_byte = 0;
    }
}

BinReader::BinReader(istream& stream): cur_byte(0), pos(0), stream(stream) {}

void BinReader::read(char* buffer, size_t cnt)
{
    assert(pos == 0);
    stream.read(buffer, cnt);
}
bool BinReader::read_bit()
{
    if (pos == 0)
        read(&cur_byte, 1);

    bool ans = (cur_byte & (1 << pos));

    pos++;
    if (pos == 8)
        pos = 0;

    return ans;
}

ostream& operator<< (ostream &s, CompressionStatistic data)
{
    cout << data.original << endl;
    cout << data.compressed << endl;
    cout << data.add_inf << endl;
    return s;
}

CompressionStatistic zip(istream& input, ostream& output)
{
    CompressionStatistic ans;

    TableOfCounts table = cnt(input);
    input.clear();
    input.seekg(ios_base::beg);

    HuffTree tree(table);

    BinWriter out(output);
    for (size_t i = 0; i < size_char; i++)
    {
        out.write((char const*)&table[i], sizeof(table[i]));
    }
    ans.add_inf = sizeof(table);

    char c;
    while (input.read(&c, 1))
    {
        vector<bool> code = tree.get_code(c);
        for (int i = 0; i < (int)code.size(); i++)
        {
            out.write_bit(code[i]);
        }
    }
    out.flush();

    ans.compressed = (size_t) output.tellp() - ans.add_inf;
    input.clear();
    ans.original = (size_t) input.tellg();

    return ans;
}

CompressionStatistic unzip(istream& input, ostream& output)
{
    CompressionStatistic ans;

    BinReader in(input);

    TableOfCounts table;
    size_t size_table = 0;
    for (size_t i = 0; i < size_char; i++)
    {
        in.read((char*)&table[i], sizeof(table[i]));
        size_table += table[i];
    }
    ans.add_inf = sizeof(table);

    HuffTree tree(table);
    for(size_t i = 0; i < size_table; i++)
    {
        shared_ptr<TreeNode> node = tree.get_root();
        while(node->left != nullptr)
        {
            bool cur_byte = in.read_bit();
            if (cur_byte)
                node = node->right;
            else
                node = node->left;
        }
        output.write(reinterpret_cast<const char*> (&node->letter), 1);
    }

    ans.original = (size_t) output.tellp();
    input.clear();
    ans.compressed = (size_t) input.tellg() - ans.add_inf;

    return ans;
}
