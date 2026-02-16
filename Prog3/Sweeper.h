#ifndef SWEEPER_HPP
#define SWEEPER_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Params.hpp"
#include "Direntry.h"
#include "Stats.h"
#include "FileID.h"

class Sweeper {
private:
    Params &params;          // store a reference instead of copy
    std::string startName;
    std::string fullPath;
    std::vector<FileID> files;

public:
    Sweeper(Params &p);      // take by reference

    void oneDir();
    void printFiles(std::ostream &out = std::cout) const;
};

#endif