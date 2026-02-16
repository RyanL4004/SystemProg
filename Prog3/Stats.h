#ifndef STATS_HPP
#define STATS_HPP

#include <sys/stat.h>
#include <iostream>

class Stats : public stat {
public:
    Stats();
    ~Stats();

    void print(std::ostream &out = std::cout) const;
};

#endif