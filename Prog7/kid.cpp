// kid.cpp
// Kyle Mather
// Client (kid) main: opens a socket connection to mom, then delegates
// all game logic to KidLogic.
// Usage: ./kid <hostname> <name>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "socktypes.h"
#include "protocol.hpp"
#include "kidLogic.hpp"

using namespace std;

// fatalp: print a system-error message and exit.
void fatalp(const char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: kid <hostname> <name>\n";
        return 1;
    }

    const char* hostname = argv[1];
    const char* name     = argv[2];

    // -----------------------------------------------------------------------
    // Resolve hostname and connect to mom.
    // -----------------------------------------------------------------------
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) fatalp("socket");

    HOSTENT* hostInfo = gethostbyname(hostname);
    if (!hostInfo) {
        cerr << "kid: can't resolve hostname '" << hostname << "'\n";
        exit(1);
    }

    SA_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    memcpy(&serverAddr.sin_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddr.sin_port = htons(PORT);

    if (connect(sockFd, (SA*)&serverAddr, sizeof(serverAddr)) < 0)
        fatalp("connect");

    cout << name << " connected to mom at " << hostname << "\n";

    // -----------------------------------------------------------------------
    // Run the game.
    // -----------------------------------------------------------------------
    {
        KidLogic kidLogic(sockFd, name);
        kidLogic.run();
    }   // KidLogic destructor closes the dup'd stream fds here.

    close(sockFd);
    return 0;
}
