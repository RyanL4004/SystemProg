#ifndef SWEEPER_HPP
#define SWEEPER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "Params.hpp"
#include "Direntry.h"
#include "Stats.h"
#include "FileID.h"
using namespace std;

class Sweeper {
private:
    Params &params;
    string startName;
    string fullPath;
    vector<FileID> files;

    // helper to join paths safely
    static string joinPath(const string& base, const string& name) {
        if (base.empty()) return name;
        if (base.back() == '/') return base + name;
        return base + "/" + name;
    }

public:
    Sweeper(Params &p);

    // P4: recursive traversal
    void travel(const string& absDirPath);

    // sorting by size for P4
    void sortBySize();

    void printFiles(ostream &out = cout) const;
};

#endif
