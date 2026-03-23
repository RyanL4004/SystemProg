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
    inline string getName() const { return simpleName; }

    bool operator<(const FileID &other) const {
        return inode < other.inode;
    }
};

// Global comparison function for size — passed explicitly to std::sort
inline bool compareBySize(const FileID &a, const FileID &b) {
    return a.getSize() < b.getSize();
}

#endif