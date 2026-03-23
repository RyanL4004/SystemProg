// Ryan Lang & Kyle Mather
// 2/25/26
#include <iostream>
#include <unistd.h>
#include "Kid.h"

using namespace std;

Kid::Kid(Model* m, int idNum) {
    model = m;
    id = idNum;
    seatNumber = -1;
    seed = static_cast<unsigned int>(
    time(NULL) ^ id ^ reinterpret_cast<uintptr_t>(pthread_self())
    );
    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGUSR1);
    sigaddset(&signalSet, SIGUSR2);
    sigaddset(&signalSet, SIGQUIT);

    pthread_sigmask(SIG_BLOCK, &signalSet, NULL);

    pthread_create(&tid, NULL, startThread, (void*)this);
}

int Kid::getID() { return id; }

pthread_t Kid::getTid() { return tid; }

bool Kid::isSitting() { return seatNumber != -1; }

void Kid::standUp() { seatNumber = -1; }

void Kid::doMarch() {
    wantSeat = rand_r(&seed) % model->nChairs;

    pthread_mutex_lock(&model->lock);

    model->nMarching++;   // first increment

    pthread_cond_signal(&model->condMarch);
    pthread_mutex_unlock(&model->lock);

    printf("Kid %d starts marching\n", id);
}

void Kid::doSit() {
    usleep(rand_r(&seed) % 500000);

    pthread_mutex_lock(&model->lock);

    model->nMarching++;   // second increment

    pthread_cond_signal(&model->condStop);
    pthread_mutex_unlock(&model->lock);

    int checked = 0;
    int index = wantSeat;

    while (checked < model->nChairs) {

        // SPOT chair outside lock
        if (model->chairs[index] == -1) {

            pthread_mutex_lock(&model->lock);

            // DOUBLE CHECK after locking
            if (model->chairs[index] == -1) {
                model->chairs[index] = id;
                seatNumber = index;

                printf("Kid %d got chair %d\n", id, index);

                pthread_mutex_unlock(&model->lock);
                return;
            }

            pthread_mutex_unlock(&model->lock);
        }

        index = (index + 1) % model->nChairs;
        checked++;
    }

    printf("Kid %d is OUT\n", id);
    pthread_exit(NULL);
}

void Kid::takeTurn(int sig) {
    if (sig == SIGUSR1)
        doMarch();
    else if (sig == SIGUSR2)
        doSit();
}

void Kid::play() {
    int sig;

    while (true) {
        sigwait(&signalSet, &sig);

        if (sig == SIGQUIT)
            pthread_exit(NULL);

        takeTurn(sig);
    }
}


void* startThread(void* kid) {
    Kid* k = (Kid*)kid;
    k->play();
    return NULL;
}

const char* sigName(int sig) {
    switch (sig) {
        case SIGUSR1: return "SIGUSR1";
        case SIGUSR2: return "SIGUSR2";
        case SIGQUIT: return "SIGQUIT";
        default: return "Unexpected signal";
    }
}