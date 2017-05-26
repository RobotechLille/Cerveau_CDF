#include <TimedPID.h>
#include <SimpleTimer.h>
#include <math.h>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// COMMANDES QUI PEUVENT ÊTRE REÇUES OU ENVOYÉES
// Les 3 premières servent aussi pour l'état du PID
#define AVANCER_RECULER 0
#define TOURNER 1
#define STOP 2
#define FUNNY_ACT 5
#define TIRETTE 6
#define OBST_ON 7
#define OBST_OFF 8

//variable globale de choix de commande
int etat_pid = STOP;

//Définition des pins des signaux A et B des codeuses
#define pinA_G  2     // int0
#define pinA_D  3     // int1
#define pinB_G  4
#define pinB_D  8

#define pinTirette 51

//Définitions des pins de commande moteur
#define pin_cmd_G 11 //9 uno //11 méga
#define pin_rot_G 7
#define pin_cmd_D 12   // 10 uno // 12 méga
#define pin_rot_D 5

// Define the PID controller proportional, integral and derivative gains
float Kp = 0.8;
float Ki = 0.001;
float Kd = 0.01;

//Variables PID
int long tick_G = 0;// Compteur de tick de la codeuse gauche
int long tick_D = 0;// Compteur de tick de la codeuse droite
int long tick_G_prec = 0;
int long tick_D_prec = 0;
int long consigne_G = 0 ;
int long consigne_D = 0;
int pwm_g =0, pwm_d = 0;
// Create a TimedPID object instance
TimedPID pid_G(Kp, Ki, Kd);
TimedPID pid_D(Kp, Ki, Kd);

float force;        // the force applied to the mass by the control system

// Define the maximum force the system can apply
float fMax = 80;

// Variables used for set point control and time step calculation
unsigned long initialTime;
unsigned long lastTime;

//variables de timer
#define frequence_echantillonnage     20
SimpleTimer timer;
int timer_T;
int trig = 53;
int echo = 52;
int trigAr = 53;
int echoAr = 52;
long lecture_echo;
long cm;

float rayon = 4.0;
float entreroues = 24;
float perimetre = M_2_PI * rayon;
float nbTicks = 1024;
float reducteur = 4;



// Vérifie le capteur
bool obstacle() {
    int t;
    int e;
    if (true) { // TODO Si on avance
        t = trig;
        e = echo;
    } else {
        t = trigAr;
        e = echoAr;
    }
    digitalWrite(t, HIGH);
    delayMicroseconds(10);
    digitalWrite(t, LOW);
    lecture_echo = pulseIn(e, HIGH);
    cm = lecture_echo / 58;
    return cm < 12;
}

//--------------------------------------------------------------------------------------------------------
/* Fonctions pour la communication série */
void floatToStr(float f, char r[9]) {
    char s[255];
    // sprintf(s, "%8f", (double) f); // Ne marche pas sur Arduino
    dtostrf(f, 8, 8, s);
    int i;
    for (i = 0; i < 8; i++) {
        r[i] = s[i];
    }
    r[8] = '\0';
}

float Serialreadfloat()
{
    char BUFFER[8]={'0'};
    while (Serial.available()<8){}
    for(byte i = 0; i < 8; i++)
    {
        BUFFER[i] = Serial.read();
        delay(5);
    }
    return atof(BUFFER);
}

void Serialsendfloat(float a)
{
    char retour[9] = "123.4567";
    floatToStr(a, retour);
    int i;
    for (i = 0; i < 8; i++) {
        Serial.write(retour[i]);
        delay(5);
    }
}

float conversion = (M_PI*8)/1024;
//--------------------------------------------------------------------------------------------------------
/* Arrêt du PID et retour sur ce qui a été fait  */
void retourConsigne()
{
    float retour; // Distance parcourue ou angle tourné
    if (etat_pid == AVANCER_RECULER)
    {
        retour = perimetre * reducteur * (tick_G - tick_G_prec + tick_D - tick_D_prec) / 2 / nbTicks;
    }
    else if (etat_pid == TOURNER)
    {
        retour = (perimetre * reducteur * (tick_G - tick_G_prec - tick_D + tick_D_prec) / 2 / nbTicks) / entreroues * 2;
    }
    else
    {
        return;
    }
    Serial.write(etat_pid);
    etat_pid = STOP;
    delay(5);
    Serialsendfloat(retour);
}

bool obstDet = true;


//--------------------------------------------------------------------------------------------------------
/* Actualisation du PID (sur timer)  */
void asservissement()
{
    int etat_pid_temp = etat_pid;

    if (obstDet && obstacle()) {
        etat_pid_temp = STOP;
    }

    //  Paramètrage du PID
    switch(etat_pid_temp){
        case AVANCER_RECULER :
            // Kp = 0.08;
            // Ki = 0.001;
            // Kd = 0.01;
            pwm_g = abs(pid_G.getCmdAutoStep(abs(consigne_G), tick_G));
            if(consigne_G <0)
            {
                digitalWrite(pin_rot_G,HIGH);
                delay(1);
                digitalWrite(pin_rot_D,LOW);
                delay(1);
            }
            else {
                digitalWrite(pin_rot_G,LOW);
                delay(1);
                digitalWrite(pin_rot_D,HIGH);
                delay(1);
            }
            break;

        case TOURNER :
            // Kp = 0.2;
            // Ki = 0.00025;
            // Kd = 0.0025;
            pwm_g = pid_G.getCmdAutoStep(abs(consigne_G), tick_G);
            if(consigne_G <0)
            {
                digitalWrite(pin_rot_G,HIGH);
                delay(1);
                digitalWrite(pin_rot_D,HIGH);
                delay(1);
            }
            else {
                digitalWrite(pin_rot_G,LOW);
                delay(1);
                digitalWrite(pin_rot_D,LOW);
                delay(1);
            }
            pwm_g = abs(pwm_g);
            break;

        case STOP :
            pwm_g = 0;
            pwm_d =0 ;
            break;
    }

    // Mise à jour du PID
    analogWrite(pin_cmd_G,pwm_g);
    delay(1);
    analogWrite(pin_cmd_D,pwm_g);
    delay(1);

    // Si on est arrivé à la consigne
    if (abs(tick_G - consigne_G) <= 10) {
    // if (consigne_G - 10 <= tick_G) {
        retourConsigne();
    }
}


//--------------------------------------------------------------------------------------------------------
/* Interruption sur tick des codeuses */
void compteur_gauche()
{
    if (digitalRead(pinB_G)==LOW)
    {
        tick_G--;  // On incrémente le nombre de tick de la codeuse
    }
    else
    {
        tick_G++;
    }
}

void compteur_droit()
{
    if (digitalRead(pinB_D)==LOW)
    {
        tick_D++;  // On incrémente le nombre de tick de la codeuse
    }
    else
    {
        tick_D--;
    }
}


//--------------------------------------------------------------------------------------------------------
/* Interruption sur evenement serial */
void serialEvent() {
    while(Serial.available())  // Tant qu'il y a des choses à lire (vu qu'une interruption désactive les autres)
    {
        int cmd = Serial.read();
        delay(1);
        switch (cmd) {
            case AVANCER_RECULER :
                tick_G_prec = tick_G;
                tick_D_prec = tick_D;
                etat_pid = AVANCER_RECULER;
                //On recoit alors des centimetres qu'on convertit en ticks
                consigne_G = nbTicks * Serialreadfloat() / perimetre / reducteur;
                consigne_D = consigne_G;
                break;

            case TOURNER :
                tick_G_prec = tick_G;
                tick_D_prec = tick_D;
                etat_pid = TOURNER;
                //On recoit alors des degres qu'on convertit en ticks
                consigne_G = nbTicks * (entreroues * Serialreadfloat() / 2) / perimetre / reducteur;
                consigne_D = -consigne_G;
                break;

            case STOP :
                retourConsigne();
                etat_pid = STOP;
                consigne_G = 0;
                consigne_D = 0;
                break;

            case FUNNY_ACT:
                etat_pid = STOP; // On est censé recevoir un STOP avant mais sais-t-on jamais
                myservo.write(250);
                delay(15);
                Serial.write(FUNNY_ACT);
                break;

            // Attend jusqu'à la tirette
            case TIRETTE:
                etat_pid = STOP; // On est censé ne pas avoir démarré mais sais-t-on jamais
                while(!digitalRead(pinTirette)){}
                Serial.write(TIRETTE);
                break;

            case OBST_ON:
                obstDet = true;
                break;

            case OBST_OFF:
                obstDet = false;
                break;

        }
    }
}

void setup() {
    myservo.attach(9);
    // Remise à zéro de la funny action
    myservo.write(25);
    delay(15);

    //set sensor
    pinMode(trig, OUTPUT);
    digitalWrite(trig, LOW);
    pinMode(echo, INPUT);

    //set sensor arrière
    pinMode(trigAr, OUTPUT);
    digitalWrite(trigAr, LOW);
    pinMode(echoAr, INPUT);

    //Set PWM frequency to 31kHz for D11 & D12 !!!FOR Arduino Mega only
    TCCR1B = TCCR1B & B11111000 | B00000001;
    // Open Serial communication and wait for monitor to be opened
    Serial.begin(9600);
    while(!Serial);

    // Set the PID controller command range
    pid_G.setCmdRange(-fMax, fMax);

    // Définition des pins et des routines d'interruption
    attachInterrupt(0, compteur_gauche, RISING);    // Appel de compteur_gauche sur front montant valeur signal A de la codeuse gauche (interruption 0 = pin2 arduino mega)
    attachInterrupt(1, compteur_droit, RISING);     // Appel de compteur_droit sur front montant valeur signal A de la codeuse droite (interruption 1 = pin3 arduino mega)
    pinMode(pinB_G, INPUT);   // Pin de signal B du la codeuse gauche
    pinMode(pinB_D, INPUT);   // Pin de signal B de la codeuse droite

    // Liaison avec les résistances de Pull-up pour les interuptions
    digitalWrite(pinA_G,HIGH);
    digitalWrite(pinA_D,HIGH);
    digitalWrite(pinB_G,HIGH);
    digitalWrite(pinB_D,HIGH);

    pinMode(pin_cmd_G,OUTPUT);
    pinMode(pin_rot_G,OUTPUT);
    pinMode(pin_cmd_D,OUTPUT);
    pinMode(pin_rot_D,OUTPUT);

    //tirette
    pinMode(pinTirette, OUTPUT);
    digitalWrite(pinTirette,HIGH) ;
    // Initialisation du timer qui lance le calcul PID (fonction asservissement) à la fréquence d'échantillonage
    timer_T = timer.setInterval(1000/frequence_echantillonnage, asservissement);
    delay(1);
    timer.enable(timer_T);
    delay(1);
}

void loop() {
    timer.run();
}

