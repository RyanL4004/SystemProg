#include "Direntry.h"
#include <string>
#include <iostream>
#include <cstring>

// Constructor / Destructor
Direntry::Direntry() = default;
Direntry::~Direntry() = default;

// Print function
void Direntry::print(std::ostream &out) const {
    std::string typeStr;
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
std::string Direntry::getName() const { return std::string(d_name); }