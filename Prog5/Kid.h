// Ryan Lang & Kyle Mather
// 2/25/26
#ifndef KID_H
#define KID_H

#include <pthread.h>
#include <signal.h>
#include "Model.h"

class Kid {
private:
    Model* model;
    int id;
    pthread_t tid;
    unsigned int seed;
    sigset_t signalSet;
    int wantSeat;
    int seatNumber;

public:
    Kid(Model* m, int id);
    ~Kid() {}
    int getID();
    pthread_t getTid();
    bool isSitting();
    void standUp();
    void takeTurn(int sig);
    void doMarch();
    void doSit();
    void play();
};

void* startThread(void* kid);
const char* sigName(int sig);

#endif