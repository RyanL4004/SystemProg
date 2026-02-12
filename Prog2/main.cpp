#include "Params.hpp"
#include <iostream>

int main(int argc, char** argv) {
    try {
        Params params(argc, argv);
        params.print();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
