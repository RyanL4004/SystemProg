#include <iostream>
#include <unistd.h>
#include "Params.hpp"
#include "Sweeper.h"
#include "tools.c"


int main(int argc, char* argv[]) {
    banner();
    cout << "Welcome to P3: Sweeper\n";

    Params params(argc, argv);
    Sweeper sweeper(params);

    if (chdir(params.startDir) != 0) {
        perror("chdir failed");
        return 1;
    }

    sweeper.oneDir();
    sweeper.printFiles(params.out.is_open() ? params.out : cout);

    bye();
    return 0;
}