#include <stdio.h>
#include <stdlib.h>
#include "chef.h"



//TODO définir la position initiale du robot
#define X_Jinit
#define Y_Jinit

#define X_Binit
#define Y_Binit





//Variables Globales

coor PositionR,PositionA;
int orientationRobot;
int* terrain[2];
int Equipe;
int phase;
//FONCTIONS
//// GESTION TERRAIN

void init_Terrain()
{
    int i=0,j=0;
    terrain[0]= malloc(300*sizeof(int));
    terrain[1]= malloc(200*sizeof(int));
    for(i=0;i<300;i++)
    {
        for(j=0;j<200;j++)
        {

            //TODO :: Si tel zone et ben on dit LIBRE / DANGER / INTERDITE
            //voir page 32 reglement

        }
    }
}

void free_Terrain()
{
    free(terrain[0]);
    free(terrain[1]);
}


//// direction fonctions

int envoi_donnée_moteur(); //TODO 2octets un pour le type de mouvement un pour la distance ou l'angle a faire
int tourner(int angle)
{
    //TODO envoyer les donner de virage au moteur
    orientationRobot+= angle;

    coor PositionR = getIMUdata();
}
int deplacer(float x, float y)
{
    coor pos_actuel = PositionA;
    coor refIMU = getIMUdata() //TODO fonction de recuperation des données IMU donc de la Position
    // x = distanceparcourue*cos(orientationR)
    // y = distanceparcourue*sin(orientationRobot)
    //transformation du déplacement imu en déplacement sur le repere général.
    // sinon on peut envoyer directement la distance a parcourir
    //faire la différence pour obtenir le déplacement en centimetre effecctué dans l'orientation

}

int recupererObjet();
int lacherObjet();
int choisirCouleur();

int main() {

    printf("Hello world!"); //Voila fini :p
    init_Terrain();
    while(/*temps*/ <90)
    {
        if(phase == 1)
        {
            //TODO fonctions correspondantes a la phase 1
            // voir si en une ou plusieurs
        }
        else if (phase == 2)
        {

            //TODO fonctions correspondantes a la phase 2
            // voir si en une ou plusieurs

        }

    }

    free_Terrain();
    return 0;
}
