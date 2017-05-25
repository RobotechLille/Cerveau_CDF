#include "moteur.h"
#include "common.h"

// MOTEUR

void avancer(float dist)
{
    // Envoi de l'ordre
    sendChar(0);

    // Envoi de la consigne
    sendFloat(dist);
}

void pivoter(float angle)
{
    // Envoi de l'ordre
    sendChar(1);

    // Envoi de la consigne
    sendFloat(angle);
}

float lireFinal()
{
    readChar();
    float final = 42;
    final = readFloat();
    return final;
}

// PRINCIPAL

int main()
{
    printf("Initialisation...\n");
    ard = openRetry("com/ardMoteur", O_RDWR | O_NOCTTY);
    serialConfig(ard, B9600);
    sleep(1);
    printf("Arduino connecté\n");

    avancer(30);
    printf("Final = %f\n", lireFinal());

    sleep(20);

    printf("Fin\n");
    close(ard);
}

