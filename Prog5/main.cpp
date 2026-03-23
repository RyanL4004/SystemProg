#include <iostream>
#include "Params.hpp"
#include "Sweeper.h"
#include "tools.h"

int main(int argc, char* argv[]) {
    banner();
    std::cout << "Welcome to P5: Sweeper (Duplicate Finder)\n";

    Params params(argc, argv);
    Sweeper S(params);

    // Recursive traversal
    S.travel(params.startDir);

    // Print all files found (sorted by size)
    S.sortBySize();
    std::ostream &out = params.out.is_open() ? params.out : std::cout;
    S.printFiles(out);

    // P5: find and report duplicates
    S.findDups(S.getFiles(), out);

    bye();
    return 0;
}