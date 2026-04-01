// kidLogic.cpp
// Kyle Mather
// Implements KidLogic: state-machine protocol handler for a kid (client).

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "kidLogic.hpp"

using namespace std;

// Must match the StateT enum order.
const char* KidLogic::stateName[] = {
    "NEWBIE", "MARCHING", "SEEKING", "SITTING", "QUIT"
};

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

KidLogic::KidLogic(int fd, const char* name)
    : momFd(fd), nChairs(0), kidName(name), pcol(NEWBIE)  // NOLINT
{
    // Duplicate fd so each stream owns an independent descriptor.
    int fdIn  = dup(fd);
    int fdOut = dup(fd);
    if (fdIn < 0 || fdOut < 0) fatalp("dup failed in KidLogic constructor");

    momIn  = fdopen(fdIn,  "r");
    momOut = fdopen(fdOut, "w");
    if (!momIn || !momOut) fatalp("fdopen failed in KidLogic constructor");
}

KidLogic::~KidLogic() {
    if (momOut) fclose(momOut);
    if (momIn)  fclose(momIn);
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

void KidLogic::run() {
    srand(static_cast<unsigned>(time(nullptr) ^ getpid()));

    // Send HELLO immediately after connecting.
    fprintf(momOut, "HELLO %s\n", kidName.c_str());
    fflush(momOut);
    cout << kidName << " sent: HELLO\n";

    // Dispatch commands until mom tells us to stop.
    while (pcol != QUIT) {
        doCommand();
    }
}

// ---------------------------------------------------------------------------
// Command dispatcher
// ---------------------------------------------------------------------------

void KidLogic::doCommand() {
    int status = fscanf(momIn, "%6s", command);
    if (status != 1) fatalp("Error reading command from mom");

    cout << kidName << " | State=" << stateName[pcol]
         << " | Command=" << command << "\n";

    try {
        if      (strcmp("GETUP", command) == 0) doGetup();
        else if (strcmp("SIT",   command) == 0) doSit();
        else if (strcmp("NACK",  command) == 0) doNack();
        else if (strcmp("ACK",   command) == 0) doAck();
        else if (strcmp("QUIT",  command) == 0) doQuit();
        else if (strcmp("PRIZE", command) == 0) doPrize();
        else throw string("Protocol is mixed up.");
    }
    catch (string& s) {
        cout << s << " [" << command << "]\n";
        exit(1);
    }
}

// ---------------------------------------------------------------------------
// Protocol action functions
// ---------------------------------------------------------------------------

// GETUP <nchairs>  — store chair count and start marching.
void KidLogic::doGetup() {
    int got = fscanf(momIn, "%d", &nChairs);
    if (got != 1) throw string("doGetup: can't read nChairs");
    cout << kidName << " starts marching! (nChairs=" << nChairs << ")\n";
    pcol = MARCHING;
}

// SIT  — music stops; pick a random chair and send WANT.
void KidLogic::doSit() {
    // Random delay so kids don't all grab the same chair.
    usleep(rand() % 30000);

    int chair = rand() % nChairs;
    fprintf(momOut, "WANT %d\n", chair);
    fflush(momOut);
    cout << kidName << " wants chair " << chair << "\n";
    pcol = SEEKING;
}

// ACK  — seat granted; wait for next round or PRIZE.
void KidLogic::doAck() {
    cout << kidName << " got a seat! Sitting happily.\n";
    pcol = SITTING;
}

// NACK <chairs>  — chair taken; pick the first available and retry.
void KidLogic::doNack() {
    int got = fscanf(momIn, "%s", chairStr);
    if (got != 1) throw string("doNack: can't read chairs string");

    char* avail = strchr(chairStr, '1');
    if (!avail) throw string("doNack: no available chair in NACK string");

    int chair = static_cast<int>(avail - chairStr);
    fprintf(momOut, "WANT %d\n", chair);
    fflush(momOut);
    cout << kidName << " retrying — wants chair " << chair
         << " (available: " << chairStr << ")\n";
    // Stay in SEEKING state.
}

// QUIT  — eliminated; set state so run() exits.
void KidLogic::doQuit() {
    cout << kidName << " is OUT. So sad...\n";
    pcol = QUIT;
}

// PRIZE  — winner! Set state so run() exits.
void KidLogic::doPrize() {
    cout << kidName << " WINS THE PRIZE! Hooray!\n";
    pcol = QUIT;
}
