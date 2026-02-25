#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include "Kid.h"


using namespace std;

void playOneRound(Model* m, Kid* players[]) {
    printf("\n-------------------- NEW ROUND --------------------\n");

    pthread_mutex_lock(&m->lock);

    for (int i = 0; i < m->nChairs; i++)
        m->chairs[i] = -1;

    m->nMarching = 0;

    int currentPlayers = m->nPlayers;

    pthread_mutex_unlock(&m->lock);

    for (int i = 0; i < currentPlayers; i++) {
        players[i]->standUp();
        pthread_kill(players[i]->getTid(), SIGUSR1);
    }

    // Wait until all kids start marching
    pthread_mutex_lock(&m->lock);
    while (m->nMarching < currentPlayers)
        pthread_cond_wait(&m->condMarch, &m->lock);
    pthread_mutex_unlock(&m->lock);

    // Slow it down
    sleep(1);

    // Send stop signal
    for (int i = 0; i < currentPlayers; i++)
        pthread_kill(players[i]->getTid(), SIGUSR2);

    // Wait until all kids stop marching
    pthread_mutex_lock(&m->lock);
    while (m->nMarching < 2 * currentPlayers)
        pthread_cond_wait(&m->condStop, &m->lock);
    pthread_mutex_unlock(&m->lock);

    int loserIndex = -1;

    for (int i = 0; i < currentPlayers; i++) {
        if (!players[i]->isSitting()) {
            loserIndex = i;
            break;
        }
    }

    if (loserIndex != -1) {
        printf("Mom removes Kid %d\n", players[loserIndex]->getID());

        delete players[loserIndex];

        players[loserIndex] = players[currentPlayers - 1];

        m->nPlayers--;
        m->nChairs--;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./musical <nKids>\n";
        return 1;
    }

    int nKids = atoi(argv[1]);

    Model model(nKids - 1, nKids);

    srand(time(NULL));

    Kid** players = new Kid*[nKids];

    for (int i = 0; i < nKids; i++)
        players[i] = new Kid(&model, i);

    while (model.nPlayers > 1)
        playOneRound(&model, players);

    printf("\nWINNER: Kid %d\n", players[0]->getID());
    pthread_kill(players[0]->getTid(), SIGQUIT);

    for (int i = 0; i < nKids; i++)
        pthread_join(players[i]->getTid(), NULL);

    delete[] players;

    pthread_exit(NULL);
}