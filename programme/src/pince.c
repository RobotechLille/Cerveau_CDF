#include "pince.h"
#include "serial.h"
#include "common.h"

int main() {
    char signal = 42;

    printf("8\n");
    // Initialisation de la communication interne
    // int pinceEff = openRetry("com/pinceEff", O_RDONLY); // Eff = cerveau → effecteurs
    // printf("10\n");
    // int pinceCap = openRetry("com/pinceCap", O_WRONLY); // Cap = capteurs → cerveau
    // printf("11\n");

    // Initialisation de la communication externe
    int ard = openRetry("com/ardPince", O_RDWR | O_NOCTTY);
    printf("11\n");
    serialConfig(ard, B9600);
    printf("12\n");

    for (;;) {
        // On reçoit le signal d'ouverture / fermeture du chef
        // read(pinceEff, &signal, sizeof(signal));
        signal = 'a'; // DEBUG
        if (signal < 0) continue;
        printf("13\n");

        // Transmission à l'Arduino
        write(ard, &signal, sizeof(signal));
        printf("30\n");

        // On attend la fin de l'action
        char part;
        read(ard, &part, sizeof(part));
        printf("35\n");

        if (part == 0) {
            signal = PINCE_FINI;
        } else {
            signal = SIGNAL_PLS;
        }
        printf("Signal %d", signal); // DEBUG
        // write(pinceCap, &signal, sizeof(signal));

    }

    close(ard);
    // close(pinceEff);
    // close(pinceCap);

}

