#include <iostream>
#include "Params.hpp"
#include "Sweeper.h"
#include "tools.h"

int main(int argc, char* argv[]) {
    banner();
    std::cout << "Welcome to P4: Sweeper (Recursive Traversal)\n";

    Params params(argc, argv);
    Sweeper S(params);

    // P4: recursive traversal starting at absolute path params.startDir
    S.travel(params.startDir);

    // P4: sort by length/size
    S.sortBySize();

    // output
    S.printFiles(params.out.is_open() ? params.out : std::cout);

    bye();
    return 0;
}
