#ifndef DIRENTRY_HPP
#define DIRENTRY_HPP

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
using namespace std;

class Direntry : private dirent {
public:
    // Constructor
    Direntry();
    // Destructor
    ~Direntry();

    // Print function
    void print(ostream &out = cout) const;

    // Accessors
    ino_t getInode() const;
    uint8_t getType() const;
    string getName() const;
};

#endif
