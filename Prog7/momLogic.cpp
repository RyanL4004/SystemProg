// momLogic.cpp
// Kyle Mather
// Implements MomLogic: game phases, round management, and polling loop.

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "momLogic.hpp"

using namespace std;

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

MomLogic::MomLogic(int n, POLLFD* s) : nKids(n), nAlive(n), socks(s), nChairs(0) {
    players = new Player[nKids];
    // Worst case chairs string: nKids-1 chairs + null terminator
    chairs = new char[nKids + 1];

    for (int k = 0; k < nKids; k++) {
        // Duplicate the fd so each stream owns an independent file descriptor.
        // The original socks[k].fd is kept for poll().
        int fdIn  = dup(socks[k].fd);
        int fdOut = dup(socks[k].fd);
        if (fdIn < 0 || fdOut < 0) fatalp("dup failed in MomLogic constructor");

        players[k].kidIn  = fdopen(fdIn,  "r");
        players[k].kidOut = fdopen(fdOut, "w");
        if (!players[k].kidIn || !players[k].kidOut)
            fatalp("fdopen failed in MomLogic constructor");

        players[k].alive = true;
    }
}

MomLogic::~MomLogic() {
    for (int k = 0; k < nKids; k++) {
        if (players[k].kidOut) fclose(players[k].kidOut);
        if (players[k].kidIn)  fclose(players[k].kidIn);
    }
    delete[] players;
    delete[] chairs;
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

void MomLogic::run() {
    // --- Phase 1: collect HELLO from every kid ---
    char cmd[16], name[64];
    for (int k = 0; k < nKids; k++) {
        int got = fscanf(players[k].kidIn, "%15s %63s", cmd, name);
        if (got != 2 || strcmp(cmd, "HELLO") != 0)
            fatalp("Expected HELLO from kid");
        players[k].name = name;
        cout << "Mom received: HELLO from " << name << "\n";
    }

    // --- Phase 2: play rounds until one kid remains ---
    while (nAlive > 1) {
        initRound();
        stopTheMusic();
    }

    // --- Send PRIZE to the last survivor ---
    for (int k = 0; k < nKids; k++) {
        if (!players[k].alive) continue;
        cout << "Mom sends: PRIZE -> " << players[k].name
             << " (WINNER!)\n";
        fprintf(players[k].kidOut, "PRIZE\n");
        fflush(players[k].kidOut);
        break;
    }
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void MomLogic::initRound() {
    nChairs = nAlive - 1;

    // Reset chairs: all available ('1'), plus null terminator.
    for (int i = 0; i < nChairs; i++) chairs[i] = '1';
    chairs[nChairs] = '\0';

    cout << "\n========== NEW ROUND: " << nAlive << " kids, "
         << nChairs << " chairs ==========\n";

    for (int k = 0; k < nKids; k++) {
        if (!players[k].alive) continue;
        fprintf(players[k].kidOut, "GETUP %d\n", nChairs);
        if (fflush(players[k].kidOut) != 0)
            fatalp("Lost connection to kid during GETUP");
        cout << "Mom sends: GETUP " << nChairs
             << " -> " << players[k].name << "\n";
    }
}

void MomLogic::stopTheMusic() {
    sleep(1);   // let the kids march for a bit

    // Signal all alive kids to sit.
    for (int k = 0; k < nKids; k++) {
        if (!players[k].alive) continue;
        fprintf(players[k].kidOut, "SIT\n");
        if (fflush(players[k].kidOut) != 0)
            fatalp("Lost connection to kid during SIT");
        cout << "Mom sends: SIT -> " << players[k].name << "\n";
    }

    // Poll loop: keep reading WANT messages until a kid is eliminated.
    while (true) {
        int ret = poll(socks, nKids, 5000);
        if (ret < 0) fatalp("poll error in stopTheMusic");
        if (ret == 0) continue;   // timeout — keep waiting
        if (checkSockets()) break;
    }
}

// Check each socket once per call. Returns true when the round ends
// (i.e., when one kid has been told to QUIT).
bool MomLogic::checkSockets() {
    for (int k = 0; k < nKids; k++) {
        if (!players[k].alive)          continue;
        if (!(socks[k].revents & POLLIN)) continue;
        socks[k].revents = 0;   // consume this event

        // Read WANT <chair> from kid k.
        char cmd[16];
        int  chair;
        int  got = fscanf(players[k].kidIn, "%15s %d", cmd, &chair);
        if (got != 2 || strcmp(cmd, "WANT") != 0) {
            cerr << "Protocol error: expected WANT from "
                 << players[k].name << "\n";
            fatalp("Protocol error in checkSockets");
        }
        cout << "Mom received: WANT " << chair
             << " from " << players[k].name << "\n";

        // Validate chair index.
        if (chair < 0 || chair >= nChairs) {
            // Treat out-of-bounds as a taken chair; pick first available.
            chair = 0;
        }

        if (chairs[chair] == '1') {
            // Chair is available — grant it.
            chairs[chair] = '0';
            fprintf(players[k].kidOut, "ACK\n");
            fflush(players[k].kidOut);
            cout << "Mom sends: ACK -> " << players[k].name
                 << " (chair " << chair << ")\n";

        } else {
            // Chair is taken — decide NACK or QUIT.
            if (strchr(chairs, '1') == nullptr) {
                // All chairs are filled: this kid is eliminated.
                fprintf(players[k].kidOut, "QUIT\n");
                fflush(players[k].kidOut);
                cout << "Mom sends: QUIT -> " << players[k].name
                     << "  *** ELIMINATED ***\n";
                players[k].alive = false;
                socks[k].fd *= -1;   // exclude from future polling
                nAlive--;
                return true;         // round is over
            } else {
                // Chairs remain — tell kid which ones are still free.
                fprintf(players[k].kidOut, "NACK %s\n", chairs);
                fflush(players[k].kidOut);
                cout << "Mom sends: NACK " << chairs
                     << " -> " << players[k].name << "\n";
            }
        }
    }
    return false;
}
