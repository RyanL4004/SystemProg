#ifndef FILEID_HPP
#define FILEID_HPP

#include <string>
#include <iostream>
#include "Stats.h"
using namespace std;

class FileID {
private:
    string simpleName;
    ino_t inode;
    int size;
    string fullPath;

public:
    FileID(const string &name, const string &path, const Stats &stats);

    void print(ostream &out = cout) const;

    inline string getPath() const { return fullPath; }
    inline ino_t getInode() const { return inode; }
    inline int getSize() const { return size; }
};

#endif