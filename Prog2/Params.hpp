#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <fstream>
#include <string>
#include <limits.h>   // PATH_MAX
using namespace std;

class Params {
public:
    // Data members (as required)
    char startDir[PATH_MAX];     // full pathname of starting directory
    string outPathRel;      // relative pathname of output file
    ofstream out;           // open output stream

    bool verbose;
    bool help;

    size_t minSize;              // minimum file size (>= 1)

    // Constructor
    Params(int argc, char** argv);

    // Member functions
    void usage();
    void print();
};

#endif
