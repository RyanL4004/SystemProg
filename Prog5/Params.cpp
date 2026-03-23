#include "Params.hpp"

#include <getopt.h>
#include <unistd.h>     // getcwd, chdir
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <errno.h>

// ---------- helper: parse numeric size ----------
static size_t parseMinSize(const char* s) {
    char* end = nullptr;
    errno = 0;
    unsigned long long val = std::strtoull(s, &end, 10);

    if (errno != 0 || end == s || *end != '\0') {
        throw std::runtime_error("Invalid value for --min-size");
    }
    if (val < 1) val = 1;
    return static_cast<size_t>(val);
}

// ---------- constructor ----------
Params::Params(int argc, char** argv)
    : verbose(false),
      help(false),
      minSize(1) {

    outPathRel.clear();

    const char* shortOpts = "hvd:m:o:";

    static struct option longOpts[] = {
        {"help",     no_argument,       nullptr, 'h'},
        {"verbose",  no_argument,       nullptr, 'v'},
        {"dir",      required_argument, nullptr, 'd'},
        {"min-size", required_argument, nullptr, 'm'},
        {"output",   required_argument, nullptr, 'o'},
        {nullptr,    0,                 nullptr,  0 }
    };

    std::string startDirCandidate;

    int opt;
    while ((opt = getopt_long(argc, argv, shortOpts, longOpts, nullptr)) != -1) {
        switch (opt) {
            case 'h':
                help = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'd':
                startDirCandidate = optarg;
                break;
            case 'm':
                minSize = parseMinSize(optarg);
                break;
            case 'o':
                outPathRel = optarg;
                break;
            default:
                throw std::runtime_error("Invalid command line (use --help)");
        }
    }

    // positional directory (SEARCH_ROOT)
    if (optind < argc) {
        startDirCandidate = argv[optind];
    }

    // default start directory = cwd
    if (startDirCandidate.empty()) {
        char cwd[PATH_MAX];
        if (!getcwd(cwd, sizeof(cwd))) {
            throw std::runtime_error("getcwd failed");
        }
        startDirCandidate = cwd;
    }

    // convert to absolute path
    char absPath[PATH_MAX];
    if (!realpath(startDirCandidate.c_str(), absPath)) {
        throw std::runtime_error("Invalid start directory");
    }
    std::strncpy(startDir, absPath, PATH_MAX);
    startDir[PATH_MAX - 1] = '\0';

    // open output file if specified
    if (!outPathRel.empty()) {
        out.open(outPathRel, std::ios::out | std::ios::app);
        if (!out.is_open()) {
            throw std::runtime_error("Could not open output file");
        }
    }

    // help exits early
    if (help) {
        usage();
        std::exit(0);
    }
}

// ---------- usage ----------
void Params::usage() {
    std::cout <<
        "Usage: sweeper [OPTIONS] [SEARCH_ROOT]\n\n"
        "Find duplicate files starting at SEARCH_ROOT (default: current directory).\n\n"
        "Options:\n"
        "  -d, --dir PATH         Start directory\n"
        "  -m, --min-size BYTES   Minimum file size (default: 1)\n"
        "  -o, --output FILE      Write output to FILE (append mode)\n"
        "  -v, --verbose          Verbose output\n"
        "  -h, --help             Show this help\n\n"
        "Examples:\n"
        "  sweeper\n"
        "  sweeper -v -o found.txt --dir ./testdir\n";
}

// ---------- print ----------
void Params::print() {
    std::ostream& os = out.is_open() ? out : std::cout;

    os << "Command: sweeper\n";
    os << "Start at: " << startDir << "\n";
    os << "Output file: "
       << (outPathRel.empty() ? "(stdout)" : outPathRel) << "\n";
    os << "Verbose? " << (verbose ? "Yes" : "No") << "\n";
    os << "Min size: " << minSize << "\n";
}
