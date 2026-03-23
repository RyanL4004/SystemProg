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
