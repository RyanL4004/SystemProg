#ifndef SWEEPER_HPP
#define SWEEPER_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Params.hpp"
#include "Direntry.h"
#include "Stats.h"
#include "FileID.h"
using namespace std;

class Sweeper {
private:
    Params &params;          // store a reference instead of copy
    string startName;
    string fullPath;
    vector<FileID> files;

public:
    Sweeper(Params &p);      // take by reference

    void oneDir();
    void printFiles(ostream &out = cout) const;
};

#endif