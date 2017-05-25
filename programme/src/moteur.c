#include "moteur.h"
#include "common.h"

// PRINCIPAL

int main()
{
    // Initialisation de la communication externe
    printf("Initialisation moteur...\n");
    ard = openRetry("com/ardMoteur", O_RDWR | O_NOCTTY | O_NDELAY);
    serialConfig(ard, B9600);
    sleep(1);
    printf("Arduino moteur connecté\n");

    // Initialisation de la communication interne
    int moteurEff = openRetry("com/moteurEff", O_RDONLY | O_NONBLOCK); // Eff = cerveau → effecteurs
    int moteurCap = openRetry("com/moteurCap", O_WRONLY | O_NONBLOCK); // Cap = capteurs → cerveau
    printf("Communication moteur ok\n");

    char signal;
    float consigne;
    printf("22\n");

    for (;;) {
        // Si on reçoit un message du chef
        if (read(moteurEff, &signal, 1) > 0) {
            printf("Moteur: Message du patron (%d)\n", signal);
            sendChar(signal);
            if (signal < 2) {
                read(moteurEff, &consigne, sizeof(consigne));
                printf("Moteur: Consigne de %f\n", consigne);
                sendFloat(consigne);
            }
        // Si on reçoit un message de l'Arduino
        } else if (read(ard, &signal, 1) > 0) {
            printf("Moteur: Message de l'Arduino (%d) \n", signal);
            printf("36\n");
            write(moteurCap, &signal, 1);
            printf("37\n");
            consigne = readFloat();
            printf("Moteur a lu %f\n", consigne);
            write(moteurCap, &consigne, sizeof(consigne));
        // Sinon on fout rien
        } else {
            sleep(0);
            continue;
        }
    }

    printf("Fin moteur\n");
    close(ard);
    close(moteurCap);
    close(moteurEff);
}

