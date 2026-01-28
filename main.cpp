#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

vector<string> arguments;

for (int x = 1; x < argc; x++) {
    arguments.push_back(argv[x]);
}
    cout << "All # of Arguments: " << arguments.size() << endl;
    for (size_t y = 0; y < arguments.size(); y++) {
        cout << "Argument" << ":" << arguments[y] << endl;
    }

    return 0;
}