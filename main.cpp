#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[]) {

    cout << "---------------------------------" << endl;

    string command = argv[0];
    if (command.rfind("./", 0) == 0) {
        command.erase(0, 2);
    }

    cout << left << setw(10) << "command" << command << endl;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (!arg.empty() && arg[0] == '-') {
            arg.erase(0, 1);
            cout << left << setw(10) << "switch" << arg << endl;
        }
        else {
            cout << left << setw(10) << "argument" << arg << endl;
        }
    }

    return 0;
}