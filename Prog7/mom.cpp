// mom.cpp
// Kyle Mather
// Server (mom) main: sets up the welcome socket, accepts N kids,
// then delegates all game logic to MomLogic.
// Usage: ./mom <nKids>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include "socktypes.h"
#include "protocol.hpp"
#include "momLogic.hpp"

using namespace std;

// fatalp: print a system-error message and exit.
void fatalp(const char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: mom <nKids>\n";
        return 1;
    }

    // Ignore SIGPIPE so a dead kid connection doesn't kill mom.
    // Writes to broken sockets will return -1/EPIPE instead.
    signal(SIGPIPE, SIG_IGN);

    int nKids = atoi(argv[1]);
    if (nKids < 2 || nKids > MAX_KIDS) {
        cerr << "nKids must be between 2 and " << MAX_KIDS << "\n";
        return 1;
    }

    // -----------------------------------------------------------------------
    // Create and bind the welcome socket.
    // -----------------------------------------------------------------------
    int welcomeSock = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSock < 0) fatalp("socket");

    // Allow rapid re-use of the port after an abnormal termination.
    int trueVal = 1;
    if (setsockopt(welcomeSock, SOL_SOCKET, SO_REUSEADDR,
                   &trueVal, sizeof(int)) < 0)
        fatalp("Can't set reuse socket option");

    SA_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port        = htons(PORT);

    if (bind(welcomeSock, (SA*)&serverAddr, sizeof(serverAddr)) < 0)
        fatalp("bind");

    // Large queue in case multiple kids arrive simultaneously.
    if (listen(welcomeSock, 10) < 0)
        fatalp("listen");

    cout << "Mom is ready. Waiting for " << nKids
         << " kids on port " << PORT << " ...\n";

    // -----------------------------------------------------------------------
    // Accept exactly nKids connections; store worker sockets in the poll array.
    // -----------------------------------------------------------------------
    POLLFD* socks = new POLLFD[nKids];

    for (int k = 0; k < nKids; k++) {
        SA_IN   clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int workerSock = accept(welcomeSock, (SA*)&clientAddr, &clientLen);
        if (workerSock < 0) fatalp("accept");

        socks[k].fd      = workerSock;
        socks[k].events  = POLLIN;
        socks[k].revents = 0;

        cout << "  Kid " << k + 1 << " of " << nKids << " connected.\n";
    }

    // Welcome socket no longer needed.
    close(welcomeSock);
    cout << "All " << nKids << " kids connected. Let the game begin!\n\n";

    // -----------------------------------------------------------------------
    // Run the game.
    // -----------------------------------------------------------------------
    {
        MomLogic momLogic(nKids, socks);
        momLogic.run();
    }   // MomLogic destructor closes the dup'd stream fds here.

    // -----------------------------------------------------------------------
    // Close surviving worker socket(s); skip the ones already negated.
    // -----------------------------------------------------------------------
    for (int k = 0; k < nKids; k++) {
        if (socks[k].fd < 0) continue;
        close(socks[k].fd);
    }

    delete[] socks;
    cout << "\nMom exits. Game over.\n";
    return 0;
}
