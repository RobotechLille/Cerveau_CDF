#include "common.h"
#include "moteur.h"

int moteurCap;
int moteurEff;

void avancer(float dist)
{
    char signal = MOT_AVAREC;
    write(moteurEff, &signal, 1);
    write(moteurEff, &dist, sizeof(dist));
}

void pivoter(float dist)
{
    char signal = MOT_TOURNE;
    write(moteurEff, &signal, 1);
    write(moteurEff, &dist, sizeof(dist));
}

void stop()
{
    char signal = MOT_STOP;
    write(moteurEff, &signal, 1);
}


float lireFinal()
{
    readChar();
    float final = 42;
    final = readFloat();
    return final;
}

int main() {
    // Initialisation de la communication interne
    printf("Initialisation premier...\n");
    moteurCap = openRetry("com/moteurCap", O_RDONLY | O_NONBLOCK); // Cap = capteurs → cerveau
    moteurEff = openRetry("com/moteurEff", O_WRONLY | O_NONBLOCK); // Eff = cerveau → effecteurs
    printf("Communication premier ok\n");

    avancer(30);
    char signal;
    float consigne;
    for (;;) {
        if (read(moteurCap, &signal, 1) > 0) {
            printf("Premier: Ah, signal!\n");
            if (signal == MOT_AVAREC) {
                printf("Premier: Ok, retour reçu\n");
                read(moteurCap, &consigne, sizeof(consigne));
                break;
            } else {
                perror("Premier: Signal non correspondant.\n");
            }
        /* } else if () { // SI ON A DETECTEUR DEVANT */
        } else {
            sleep(0);
            continue;
        }

    }
    printf("J'ai avancé de %f !\n", consigne);

    printf("Fin premier\n");
    close(moteurCap);
    close(moteurEff);
}
