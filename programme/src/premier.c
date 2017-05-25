#include <stdbool.h>
#include <math.h>
#include "common.h"
#include "premier.h"

// OBSTACLE

bool obstacleDevant()
{
    // TODO
    return false;
}

// MOTEUR

int ardMoteur;

// Ordres
void avancer(float dist)
{
    sendChar(ardMoteur, 0);
    sendFloat(ardMoteur, dist);
}

void pivoter(float dist)
{
    sendChar(ardMoteur, 1);
    sendFloat(ardMoteur, dist);
}

void stop()
{
    sendChar(ardMoteur, 2);
}

// COORDONNES

float xpos = 0;
float ypos = 0;
float angl = 0;

// Met à jour les coordonnées en fonction de ce qui a été avancé
void updatePos(float distReelle)
{
    printf("On a avancé de %f cm.\n", distReelle);
    xpos += cos(angl) * distReelle;
    ypos += sin(angl) * distReelle;
    printf("Nouvelle position : (%f, %f)\n", xpos, ypos);
}

bool moveForward(float dist)
{
    printf("Avançons de %f cm\n", dist);
    char signal;
    avancer(dist);

    // Boucle d'attente de signal
    for (;;)
    {
        // Si l'Arduino renvoie qu'il a terminé
        if (read(ardMoteur, &signal, sizeof(signal)) > 0) {
            if (signal != 0) {
                perror("Moteur attendait signal 0\n");
                exit(1);
            }
            updatePos(readFloat(ardMoteur));
            printf("Destination atteinte.\n");
            return true;

        // Si il y a un obstacle devant
        } else if (obstacleDevant()) {
            stop();
            printf("Stoppé pour obstacle");
            signal = readChar(ardMoteur);
            if (signal != 2) {
                perror("Moteur attendait signal 2\n");
                exit(1);
            }
            updatePos(readFloat(ardMoteur));
            return false;

        // Sinon on fout rien
        } else {
            sleep(0);
            continue;
        }
    }

}


void moveTo(float x, float y, bool avant)
{
    printf("Nouvelle destination : (%f, %f) (avant = %d)\n", x, y, avant);
    bool destAtteinte = false;
    float dist;
    while (!destAtteinte)
    {
        // TODO Trouver l'angle et s'orienter

        dist = sqrt(pow(x-xpos, 2) + pow(y-ypos, 2));
        destAtteinte = moveForward(dist);
    }
}


// PRINCIPAL

void openAll() {
    ardMoteur = openArduino("com/ardMoteur");
    printf("Tous les Arduinos sont ouverts.\n");
}

void closeAll() {
    close(ardMoteur);
    printf("Tous les Arduinos sont fermés.\n");
}

int main()
{
    openAll();

    moveForward(30);

    printf("Fin\n");
    closeAll();
}


