#include "Sweeper.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include "Params.hpp"

Sweeper::Sweeper(Params &p) : params(p) {
    startName = std::string(params.startDir);
    fullPath = startName;
}

void Sweeper::oneDir() {
    DIR* dirp = opendir(fullPath.c_str());
    if (!dirp) {
        perror("opendir failed");
        return;
    }

    if (chdir(fullPath.c_str()) != 0) {
        perror("chdir failed");
        closedir(dirp);
        return;
    }

    std::cout << "Processing directory: " << fullPath << "\n";

    dirent *entry;
    readdir(dirp); // skip "."
    readdir(dirp); // skip ".."

    while ((entry = readdir(dirp)) != nullptr) {
        Direntry de;
        std::memcpy(&de, entry, sizeof(dirent));

        uint8_t type = de.getType();
        if (type != DT_REG && type != DT_DIR) continue;

        if (params.verbose) {
            de.print();
        }

        if (type == DT_REG) {
            Stats stats;
            if (lstat(de.getName().c_str(), &stats) != 0) {
                perror("lstat failed");
                continue;
            }
            FileID fid(de.getName(), fullPath, stats);
            files.push_back(fid);
        }
    }

    closedir(dirp);
    std::cout << "Done processing directory.\n";
}

void Sweeper::printFiles(std::ostream &out) const {
    for (const auto &f : files) {
        f.print(out);
    }
}