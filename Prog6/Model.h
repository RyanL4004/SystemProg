// Ryan Lang & Kyle Mather
// 2/25/26
#ifndef MODEL_H
#define MODEL_H

#include <pthread.h>

struct Model {
    pthread_mutex_t lock;
    pthread_cond_t condMarch;
    pthread_cond_t condStop;
    int nChairs;
    int* chairs;
    int nMarching;
    int nPlayers;

    Model(int chairsCount, int playersCount) {
        nChairs = chairsCount;
        nPlayers = playersCount;
        nMarching = 0;
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&condMarch, NULL);
        pthread_cond_init(&condStop, NULL);

        chairs = new int[nChairs];
        for (int i = 0; i < nChairs; i++)
            chairs[i] = -1;
    }

    ~Model() {
        delete[] chairs;
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&condMarch);
        pthread_cond_destroy(&condStop);
    }
};

#endif