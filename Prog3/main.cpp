#include <iostream>
#include <unistd.h>
#include "Params.hpp"
#include "Sweeper.h"
#include "tools.c"
// Dummy tools functions


int main(int argc, char* argv[]) {
    banner();
    std::cout << "Welcome to P3: Sweeper\n";

    Params params(argc, argv);
    Sweeper sweeper(params);

    if (chdir(params.startDir) != 0) {
        perror("chdir failed");
        return 1;
    }

    sweeper.oneDir();
    sweeper.printFiles(params.out.is_open() ? params.out : std::cout);

    bye();
    return 0;
}