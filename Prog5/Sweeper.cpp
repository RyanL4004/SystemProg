#include "Sweeper.h"
#include <dirent.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <iostream>

Sweeper::Sweeper(Params &p) : params(p) {
    startName = string(params.startDir);  // already absolute due to Params
    fullPath  = startName;
}

void Sweeper::travel(const string& absDirPath) {
    DIR* dirp = opendir(absDirPath.c_str());
    if (!dirp) {
        cerr << "opendir failed on '" << absDirPath << "': " << strerror(errno) << "\n";
        return;
    }

    dirent* entry;

    while ((entry = readdir(dirp)) != nullptr) {
        // skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Wrap dirent in your Direntry class (same approach you used in P3)
        Direntry de;
        std::memcpy(&de, entry, sizeof(dirent));

        string name = de.getName();
        string full = joinPath(absDirPath, name);

        // Use lstat so links are recognized as links (and not followed)
        Stats stats;
        if (lstat(full.c_str(), &stats) != 0) {
            cerr << "lstat failed on '" << full << "': " << strerror(errno) << "\n";
            continue;
        }

        bool isReg  = S_ISREG(stats.st_mode);
        bool isDir  = S_ISDIR(stats.st_mode);
        bool isLink = S_ISLNK(stats.st_mode);

        // If verbose: print each entry (and include links)
        // If NOT verbose: ignore links per spec
        if (params.verbose) {
            // Your Direntry::print only prints name (not full path), so print full path too.
            cout << full << "\t";
            de.print(); // prints type, inode, d_name
        } else {
            if (isLink) continue;
        }

        // Regular file => create FileID record
        if (isReg) {
            // FileID expects: (simpleName, directoryPath, stats)
            // and it internally builds fullPath = path + "/" + name
            FileID fid(name, absDirPath, stats);

            // enforce min size filter (you already parse it in Params)
            if (static_cast<size_t>(fid.getSize()) >= params.minSize) {
                files.push_back(fid);
            }
        }
        // Directory => recurse
        else if (isDir) {
            travel(full);
        }
        // everything else ignored
    }

    closedir(dirp);
}

void Sweeper::sortBySize() {
    std::sort(files.begin(), files.end(),
              [](const FileID& a, const FileID& b) {
                  if (a.getSize() != b.getSize()) return a.getSize() < b.getSize();
                  // tie-breakers (optional but helpful for stable output)
                  if (a.getInode() != b.getInode()) return a.getInode() < b.getInode();
                  return a.getPath() < b.getPath();
              });
}

void Sweeper::printFiles(ostream &out) const {
    for (const auto &f : files) {
        f.print(out);
    }
}

void Sweeper::findDups(vector<FileID> &fileList, ostream &out) {

    // ── Pass 1: sort by iNode, find files sharing the same iNode ──────────
    out << "\n===== Duplicates by iNode (hard links) =====\n";
    std::sort(fileList.begin(), fileList.end()); // uses member operator

    bool anyInode = false;
    int i = 0;
    int n = static_cast<int>(fileList.size());

    while (i < n) {
        int j = i + 1;
        // advance j while inode matches
        while (j < n && fileList[j].getInode() == fileList[i].getInode()) {
            ++j;
        }
        // [i, j) is a group; if more than one → duplicates
        if (j - i > 1) {
            anyInode = true;
            out << "  iNode " << fileList[i].getInode() << ":\n";
            for (int k = i; k < j; ++k) {
                out << "    ";
                fileList[k].print(out);
            }
        }
        i = j; // next group
    }
    if (!anyInode) out << "  (none found)\n";


    // ── Pass 2: sort by size, compare contents of same-size files ─────────
    out << "\n===== Duplicates by content (same size + identical bytes) =====\n";
    std::sort(fileList.begin(), fileList.end(), compareBySize);

    bool anyContent = false;
    i = 0;

    while (i < n) {
        int j = i + 1;
        // advance j while size matches
        while (j < n && fileList[j].getSize() == fileList[i].getSize()) {
            ++j;
        }
        // [i, j) share the same size — compare each pair
        if (j - i > 1) {
            // check all pairs within this size group
            for (int a = i; a < j - 1; ++a) {
                for (int b = a + 1; b < j; ++b) {
                    // open and compare files byte by byte
                    ifstream fa(fileList[a].getPath(), ios::binary);
                    ifstream fb(fileList[b].getPath(), ios::binary);

                    if (!fa.is_open() || !fb.is_open()) continue;

                    bool same = true;
                    char ca, cb;
                    while (fa.get(ca) && fb.get(cb)) {
                        if (ca != cb) { same = false; break; }
                    }
                    // also check both reached EOF together
                    if (same && (fa.peek() != EOF || fb.peek() != EOF)) {
                        same = false;
                    }

                    if (same) {
                        anyContent = true;
                        out << "  Content match (size=" << fileList[a].getSize() << "):\n";
                        out << "    "; fileList[a].print(out);
                        out << "    "; fileList[b].print(out);
                    }
                }
            }
        }
        i = j;
    }
    if (!anyContent) out << "  (none found)\n";
}
