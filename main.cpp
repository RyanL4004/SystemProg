// Ryan Lang & Kyle Mather
// CSCI 4547

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {

    ofstream myOut;
    myOut.open("P1_LangMather.txt", ios::out | ios::app);

    if (!myOut) {
        cerr << "Error: Could not open the file." << endl;
        return 1;
    }

    cout << "---------------------------------" << endl;
    myOut << "---------------------------------" << endl;

    string command = argv[0];
    if (command.rfind("./", 0) == 0) {
        command.erase(0, 2);
    }

    cout << left << setw(10) << "command" << command << endl;
    myOut << left << setw(10) << "command" << command << endl;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (!arg.empty() && arg[0] == '-') {
            arg.erase(0, 1);
            cout << left << setw(10) << "switch" << arg << endl;
            myOut << left << setw(10) << "switch" << arg << endl;
        }
        else {
            cout << left << setw(10) << "argument" << arg << endl;
            myOut << left << setw(10) << "argument" << arg << endl;
        }
    }

    myOut.close();

    return 0;
}