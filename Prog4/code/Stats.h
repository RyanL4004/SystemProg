#ifndef STATS_HPP
#define STATS_HPP

#include <sys/stat.h>
#include <iostream>
using namespace std;
class Stats : public stat {
public:
    Stats();
    ~Stats();

    void print(ostream &out = cout) const;
};

#endif