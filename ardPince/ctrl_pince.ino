#include <DynamixelSerial1.h>
#include <Wire.h>
#define PINCE_GAUCHE  1      //14
#define PINCE_DROITE  2      //4

#define POSITION_FERMEE_DROITE 470
#define POSITION_FERMEE_GAUCHE 230

#define POSITION_OUVERTE_DROITE 750
#define POSITION_OUVERTE_GAUCHE 0


void fermerPince(){
   Dynamixel.moveSpeed(PINCE_DROITE,POSITION_FERMEE_DROITE,100);
   Dynamixel.moveSpeed(PINCE_GAUCHE,POSITION_FERMEE_GAUCHE,100);
}
void ouvrirPince(){
   Dynamixel.moveSpeed(PINCE_DROITE,POSITION_OUVERTE_DROITE,100);
   Dynamixel.moveSpeed(PINCE_GAUCHE,POSITION_OUVERTE_GAUCHE,100);
}

void serialEvent() {
    char command = Serial.read();
    switch (command) {
        case 'a':
            ouvrirPince();
            break;
        case 'b':
            fermerPince();
            break;
        default:
            return;
    }
    Serial.write(0x02);
}


void setup(){
  Dynamixel.begin(1000000,2);  // Inicialize the servo at 1Mbps and Pin Control 2
  delay(10);
  Serial.begin(9600); 

  Dynamixel.setEndless(PINCE_GAUCHE,OFF);
  Dynamixel.torqueStatus(PINCE_GAUCHE,OFF);
  Dynamixel.setEndless(PINCE_DROITE,OFF);
  Dynamixel.torqueStatus(PINCE_DROITE,OFF);
  ouvrirPince();
  Serial.write(0x02);
}


void loop() {
}
