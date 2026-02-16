#include "FileID.h"

FileID::FileID(const string &name, const string &path, const Stats &stats)
    : simpleName(name), inode(stats.st_ino), size(static_cast<int>(stats.st_size)) {
    fullPath = path + "/" + name;
}

void FileID::print(ostream &out) const {
    out << inode << "\t" << size << "\t" << fullPath << "\n";
}

string FileID::getPath() const { return fullPath; }
ino_t FileID::getInode() const { return inode; }
int FileID::getSize() const { return size; }