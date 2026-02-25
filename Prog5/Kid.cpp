#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "Kid.h"

using namespace std;

Kid::Kid(Model* m, int idNum) {
    model = m;
    id = idNum;
    seatNumber = -1;

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
    wantSeat = rand() % model->nChairs;

    pthread_mutex_lock(&model->lock);
    model->nMarching++;
    pthread_cond_signal(&model->condMarch);
    pthread_mutex_unlock(&model->lock);

    printf("Kid %d is marching\n", id);
}

void Kid::doSit() {
    usleep(rand() % 500000);

    pthread_mutex_lock(&model->lock);

    int checked = 0;
    int index = wantSeat;

    while (checked < model->nChairs) {
        if (model->chairs[index] == -1) {
            model->chairs[index] = id;
            seatNumber = index;
            printf("Kid %d got chair %d\n", id, index);

            model->nMarching--;
            pthread_cond_signal(&model->condStop);

            pthread_mutex_unlock(&model->lock);
            return;
        }
        index = (index + 1) % model->nChairs;
        checked++;
    }

    model->nMarching--;
    pthread_cond_signal(&model->condStop);
    pthread_mutex_unlock(&model->lock);

    printf("Kid %d is OUT\n", id);
    pthread_exit(NULL);
}

void Kid::play() {
    int sig;

    while (true) {
        sigwait(&signalSet, &sig);

        if (sig == SIGUSR1) {
            doMarch();
        }
        else if (sig == SIGUSR2) {
            doSit();
        }
        else if (sig == SIGQUIT) {
            pthread_exit(NULL);
        }
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