#include "Stats.h"

Stats::Stats() = default;
Stats::~Stats() = default;

void Stats::print(ostream &out) const {
    out << "iNode: " << st_ino
        << "\tSize: " << st_size
        << "\tLinks: " << st_nlink
        << "\tUID: " << st_uid
        << "\tGID: " << st_gid
        << "\tModified: " << st_mtimespec.tv_sec
        << "\tStatus Changed: " << st_ctimespec.tv_sec
        << "\n";
}