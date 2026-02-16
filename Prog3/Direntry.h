#ifndef DIRENTRY_HPP
#define DIRENTRY_HPP

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>

class Direntry : private dirent {
public:
    // Constructor / Destructor
    Direntry();
    ~Direntry();

    // Print function
    void print(std::ostream &out = std::cout) const;

    // Accessors
    ino_t getInode() const;
    uint8_t getType() const;
    std::string getName() const;
};

#endif
