#ifndef FILEID_HPP
#define FILEID_HPP

#include <string>
#include <iostream>
#include "Stats.h"

class FileID {
private:
    std::string simpleName;
    ino_t inode;
    int size;
    std::string fullPath;

public:
    FileID(const std::string &name, const std::string &path, const Stats &stats);

    void print(std::ostream &out = std::cout) const;

    inline std::string getPath() const;
    inline ino_t getInode() const;
    inline int getSize() const;
};

#endif