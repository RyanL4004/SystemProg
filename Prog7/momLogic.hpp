// momLogic.hpp
// Kyle Mather
// Defines MomLogic: handles all game logic after connections are established.

#ifndef MOMLOGIC_HPP
#define MOMLOGIC_HPP

#include <string>
#include <cstdio>
#include "socktypes.h"
#include "protocol.hpp"

using namespace std;

class MomLogic {
private:
    // One Player entry per kid slot.
    struct Player {
        string name  = "??";
        bool   alive = false;
        FILE*  kidIn  = nullptr;  // stream for kid -> mom
        FILE*  kidOut = nullptr;  // stream for mom -> kid
    };

    int     nKids;
    int     nAlive;
    POLLFD* socks;    // pollfd array owned by mom.cpp
    Player* players;  // dynamically allocated; freed in destructor
    char*   chairs;   // '1'=available '0'=occupied, null-terminated
    int     nChairs;

    void initRound();
    void stopTheMusic();
    bool checkSockets();   // returns true when the round is over

public:
    MomLogic(int n, POLLFD* s);
    ~MomLogic();
    void run();
};

#endif
