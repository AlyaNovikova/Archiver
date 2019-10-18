#include <iostream>
#include <fstream>
#include <array>
#include <cstring>

#include "huffman.h"

using namespace std;

struct Args
{
        string inp_file;
        string out_file;
        bool zip;
};

Args parse_arguments(int argc, char* argv[])
{
    if (argc != 6) {
        throw runtime_error("Not enought arguments");
    }

    Args args;


    bool was_mode = false;

    for (int pos = 1; pos < argc; pos++) {
        if (!strcmp(argv[pos], "-c")) {
            if (was_mode) {
                throw runtime_error("zip/unzip specifier already was in args");
            }

            args.zip = true;
            was_mode = true;
        }
        else if (!strcmp(argv[pos], "-u")) {
            if (was_mode) {
                throw runtime_error("zip/unzip specifier already was in args");
            }

            args.zip = false;
            was_mode = true;
        }
        else if (!strcmp(argv[pos], "-f") || !strcmp(argv[pos], "--file")) {
            if (args.inp_file != "") {
                throw runtime_error("input file already was in args");
            }
            pos++;
            args.inp_file = argv[pos];
        }
        else if (!strcmp(argv[pos], "-o") || !strcmp(argv[pos], "--output")) {
            if (args.out_file != "") {
                throw runtime_error("output file already was in args");
            }
            pos++;
            args.out_file = argv[pos];
        }
        else {
            throw runtime_error("unknown argument " + string(argv[pos]));
        }
    }

    return args;
}

int main(int argc, char * argv[])
{
    try {
        Args args = parse_arguments(argc, argv);

        CompressionStatistic ans;

        ifstream input(args.inp_file, ios_base::binary);
        ofstream output(args.out_file, ios_base::binary);

        if (args.zip)
            ans = zip(input, output);
        else
            ans = unzip(input, output);

        cout << ans;
    }
    catch (runtime_error const& err) {
        cout << "Error occured:" << endl;
        cout << err.what() << endl;
        return 0;
    }

    return 0;
}
