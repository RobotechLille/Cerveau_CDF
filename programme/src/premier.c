#include "premier.h"
#include "common.h"
#include <math.h>
#include <stdbool.h>
#include <signal.h>

// PINCE

void take()
{

}

void drop()
{

}

// FUNNY ACTION

void funnyAction()
{

}

// TIRETTE
void tirette() {
    // TODO Attend que la tirette soit enlevée
}

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
//

float xpos = 0;
float ypos = 0;
float angl = 0;

float deltaAngle(float xa, float ya, float xb, float yb, bool recul)
{

    float angle;
    float dx = xa - xb;
    float dy = ya - yb;
    if (dy < 0) {
        if (dx == 0) {
            angle = M_PI_2;
        } else if (dx > 0) {
            angle = (-1) * atan(fabs(dy / dx));
        } else {
            angle = -M_PI + atan(fabs(dy / dx));
        }
    } else if (dy > 0) {
        if (dx == 0) {
            angle = -M_PI_2;
        } else if (dx > 0) {
            angle = atan(fabs(dy / dx));
        } else {
            angle = M_PI - atan(fabs(dy / dx));
        }
    } else {
        if (dx > 0) {
            angle = M_PI;
        } else {
            angle = 0;
        }
    }

    if (angle > 0) {

        float ret = angl - angle;
        if (fabs(ret) <= M_PI) {
            return ((-1) * ret);
        } else {
            if (ret < 0) {
                return (M_2_PI - fabs(ret));
            } else
                return (M_2_PI - ret) * (-1);
        }
    }
    return angle;
}

// Met à jour les coordonnées en fonction de ce qui a été avancé
void updatePos(float distReelle)
{
    printf("On a avancé de %f cm.\n", distReelle);
    xpos += cos(angl) * distReelle;
    ypos += sin(angl) * distReelle;
    printf("Nouvelle position : (%f, %f)\n", xpos, ypos);
}

void updateAngl(float anglReel)
{
    printf("On a tourné de %f rad.\n", anglReel);
    angl += anglReel;
    angl = fmod(anglReel, M_2_PI);
    printf("Nouvel angle : %f\n", angl);
}

bool moveForward(float dist)
{
    printf("Avançons de %f cm\n", dist);
    char signal;
    avancer(dist);

    // Boucle d'attente de signal
    for (;;) {
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

bool rotate(float angle)
{
    char signal;
    pivoter(angle);
    signal = readChar(ardMoteur);
    if (signal != 1) {
        perror("Moteur attendait signal 1\n");
        exit(1);
    }
    updateAngl(readFloat(ardMoteur));
    return true;
}

void moveTo(float x, float y, bool avant)
{
    printf("Nouvelle destination : (%f, %f) (avant = %d)\n", x, y, avant);
    float dist;
    for (;;)
    {
        dist = deltaAngle(xpos, ypos, x, y, avant);
        if (!rotate(dist)) {
            continue;
        }

        dist = sqrt(pow(x - xpos, 2) + pow(y - ypos, 2));
        if (!moveForward(dist)) {
            continue;
        }
        break;
    }
}

// PRINCIPAL

void openAll()
{
    ardMoteur = openArduino("com/ardMoteur");
    printf("Tous les Arduinos sont ouverts.\n");
}

void closeAll()
{
    close(ardMoteur);
    printf("Tous les Arduinos sont fermés.\n");
}

void parcours()
{
    printf("Démarrage du parcours\n");
    moveTo(75, 955, true);
    moveTo(175, 955, true);
    moveTo(250, 1000, true);
    moveTo(425, 1000, true);
    take();
    moveTo(885, 1000, true);
    moveTo(885, 700, true);
    drop();
    moveTo(885, 900, false);
    moveTo(1225, 900, true);
    take();
    moveTo(1160, 900, false);
    moveTo(1311, 811, true);
    drop();
    moveTo(1160, 660, true);
}

int main()
{
    openAll();

    if (true) {
        // JAUNE
        xpos = 75;
        ypos = 2055;
        angl = M_PI;
    } else {
        // BLEU
        xpos = 75;
        ypos = 945;
        angl = -M_PI;
    }

    tirette();
    printf("Allons-y!\n");

    pid_t timer = fork();
    if (timer == 0) { // Processus enfant
        parcours();
        printf("Arrivé à la fin du parcours avant la fin du temps !\n");
    } else if (timer < 0) {
        perror("Mauvais fork\n");
        exit(1);
    } else { // Processus parent
        sleep(10); // TODO DEBUG
        printf("Temps terminé !\n");
        kill(timer, SIGKILL);
        stop();
        printf("Démarrage de la funny action...\n");
        funnyAction();
        printf("Fin\n");
        closeAll();
    }
    exit(0);
}
