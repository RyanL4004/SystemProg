#include "FileID.h"

FileID::FileID(const std::string &name, const std::string &path, const Stats &stats)
    : simpleName(name), inode(stats.st_ino), size(static_cast<int>(stats.st_size)) {
    fullPath = path + "/" + name;
}

void FileID::print(std::ostream &out) const {
    out << inode << "\t" << size << "\t" << fullPath << "\n";
}

std::string FileID::getPath() const { return fullPath; }
ino_t FileID::getInode() const { return inode; }
int FileID::getSize() const { return size; }