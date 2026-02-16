#include "Direntry.h"
#include <string>
#include <iostream>
#include <cstring>
using namespace std;

// Constructor
Direntry::Direntry() = default;
// Destructor
Direntry::~Direntry() = default;

// Print function
void Direntry::print(ostream &out) const {
    string typeStr;
    switch (d_type) {
        case DT_REG:  typeStr = "File"; break;
        case DT_DIR:  typeStr = "Directory"; break;
        case DT_LNK:  typeStr = "Link"; break;
        default:      typeStr = "Other"; break;
    }
    out << typeStr << "\t" << d_ino << "\t" << d_name << "\n";
}

// Accessors
ino_t Direntry::getInode() const { return d_ino; }
uint8_t Direntry::getType() const { return d_type; }
string Direntry::getName() const { return string(d_name); }