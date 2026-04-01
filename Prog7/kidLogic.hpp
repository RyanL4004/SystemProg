// kidLogic.hpp
// Kyle Mather
// Defines KidLogic: all protocol logic for a single kid (client).

#ifndef KIDLOGIC_HPP
#define KIDLOGIC_HPP

#include <string>
#include <cstdio>
#include "socktypes.h"
#include "protocol.hpp"

using namespace std;

class KidLogic {
private:
    // State machine for the kid's progress through the protocol.
    enum StateT { NEWBIE, MARCHING, SEEKING, SITTING, QUIT };
    static const char* stateName[];

    int    momFd;    // kept per spec; used during construction
    FILE*  momIn;
    FILE*  momOut;
    int    nChairs;
    string kidName;
    StateT pcol;
    char   command[8];         // current command word from mom
    char   chairStr[MAX_MSG];  // available-chairs string from NACK

    // Protocol action functions — one per incoming message type.
    void doGetup();
    void doSit();
    void doAck();
    void doNack();
    void doQuit();
    void doPrize();

    // Read the next command and dispatch.
    void doCommand();

public:
    KidLogic(int fd, const char* name);
    ~KidLogic();
    void run();
};

#endif
