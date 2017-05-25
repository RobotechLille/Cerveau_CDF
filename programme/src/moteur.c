#include "moteur.h"
#include "common.h"

int ard;

// FONCTIONS USUELLES

void floatToStr(float f, char r[8])
{
    char s[255];
    sprintf(s, "%8f", f);
    int i;
    for (i = 0; i < 8; i++) {
        r[i] = s[i];
    }
}

void sendChar(char c)
{
    write(ard, &c, 1);
}

void sendFloat(float f)
{
    char consigne[8];
    floatToStr(f, consigne);
    write(ard, &consigne, 8);
}

char readChar()
{
    char c;
    read(ard, &c, 1);
    return c;
}

float readFloat()
{
    char retour[9];
    int i;
    for (i = 0; i < 8; i++) {
        read(ard, &retour[i], sizeof(char));
    }
    retour[8] = '\0';
    return atof(retour);
}

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

    printf("Fin\n");
    close(ard);
}
