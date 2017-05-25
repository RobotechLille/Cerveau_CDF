
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
#define OBST_DEV 3
#define OBST_DER 4
#define FUNNY_ACT 5
#define TIRETTE 6

//variable globale de choix de commande
int etat_pid = STOP;

//Définition des pins des signaux A et B des codeuses
#define pinA_G  2     // int0
#define pinA_D  3     // int1
#define pinB_G  4 
#define pinB_D  8

#define pinTirette 51

//Définitions des pins de commande moteur
#define pin_cmd_G               11 //9 uno //11 méga
#define pin_rot_G                  7
#define pin_cmd_D                12   // 10 uno // 12 méga 
#define pin_rot_D                  5

// Define the PID controller proportional, integral and derivative gains
float Kp = 0.8;
float Ki = 0.001;
float Kd = 0.01;

//Variables PID
int long tick_G = 0;// Compteur de tick de la codeuse gauche
int long tick_D = 0;// Compteur de tick de la codeuse droite
int long tick_G_prec = 0;
int long tick_D_prec = 0;
int long consigne_G = 6000 ;
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
long lecture_echo; 
long cm;
void setup() {
  myservo.attach(9);
  myservo.write(25);
  delay(15); 

  //set sensor
  pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  
  //Set PWM frequency to 31kHz for D11 & D12 !!!FOR Arduino Mega only
  TCCR1B = TCCR1B & B11111000 | B00000001;
  // Open Serial communication and wait for monitor to be opened
  Serial.begin(9600);
  while(!Serial);

  // Set the PID controller command range
  pid_G.setCmdRange(-fMax, fMax);

  // Définition des pins et des routines d'interruption
  attachInterrupt(0, compteur_gauche, RISING);    // Appel de compteur_gauche sur front montant valeur signal A de la codeuse gauche (interruption 0 = pin2 arduino mega)
 // attachInterrupt(1, compteur_droit, RISING);     // Appel de compteur_droit sur front montant valeur signal A de la codeuse droite (interruption 1 = pin3 arduino mega)
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
  timer_T = timer.setInterval(1000/frequence_echantillonnage,asservissement);
  delay(1);
  timer.enable(timer_T);
  delay(1);
}

void loop() {
  timer.run(); 
}

void verifierCapDev() {
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58;
  if (cm < 12) {
    Serial.write(OBST_DEV);
  }
}

float conversion = (M_PI*8)/1024;
//-------------------------------------------------------------------------------------------------------- 
/* Arrêt du PID et retour sur ce qui a été fait  */
void retourConsigne()
{
  if (etat_pid == AVANCER_RECULER)
  {
  
  }
  else if (etat_pid == TOURNER)
  {
  
  }
  else
  {

  }

}


//-------------------------------------------------------------------------------------------------------- 
/* Actualisation du PID (sur timer)  */
void asservissement()
{   
 
  // Serial.print("Time - \t");
  // Serial.println((millis()-depart)/1000);
  // Serial.print("Status - \t");
  // Serial.println(status_prog);

  // Vérification du capteur devant
  verifierCapDev()
  // TODO A faire sur un autre Arduino parce que ça démonte la fréquence d'échantillonage

  //  Paramètrage du PID
  action();
  switch(etat_pid){
    case AVANCER_RECULER :
      Kp = 0.08;
      Ki = 0.001;
      Kd = 0.01;
      pwm_g = pid_G.getCmdAutoStep(consigne_G, tick_G);
      if(pwm_g <0) 
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
      pwm_g = abs(pwm_g);
      break;

    case TOURNER : 
      Kp = 0.2;
      Ki = 0.00025;
      Kd = 0.0025;
      pwm_g = pid_G.getCmdAutoStep(consigne_G, tick_G);
      pwm_d = pid_D.getCmdAutoStep(consigne_D, tick_D);
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

  /// put your main code here, to run repeatedly:
  // Serial.print("tick_g - tick_d : \t");
  // Serial.print(tick_G);
  // Serial.print("\t");
  // Serial.print(tick_D);
  // Serial.print("\t");
  // Serial.print(pwm_g);
  // Serial.print("\t");
  // Serial.println(pwm_d);

  // Si on est arrivé à la consigne
  if (consigne_G - 10 <= tick_G) {
    etat_PID = STOP;
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
    inFloat = Serialreadfloat();

    switch (cmd) {
      case AVANCER_RECULER :
	etat_pid = AVANCER_RECULER;
        //On recoit alors des centimetres qu'on convertit en ticks
        consigne_G = Serialreadfloat() * conversion;
        consigne_D = consigne_G;
        break;

      case TOURNER :
	etat_pid = TOURNER;
        //On recoit alors des degres qu'on convertit en ticks
        // TODO Conversion fausse
        consigne_G = Serialreadfloat() * conversion;
        consigne_D = -consigne_G;
        break;

      case STOP :
	etat_pid = STOP;
        consigne_G = 0;
        consigne_D = 0;
        break;

      case FUNNY_ACT:
	etat_pid = STOP; // On est censé recevoir un STOP avant mais sais-t-on jamais
        myservo.write(250);
        delay(15); 
        break;

      case TIRETTE:
	// Attend jusqu'à la tirette
  	while(!digitalRead(pinTirette)){}
	Serial.write(TIRETTE);
        break;

    }
    // Serial.write(cmd);
    // delay(5);
    // Serialsendfloat(inFloat);
  }
}
