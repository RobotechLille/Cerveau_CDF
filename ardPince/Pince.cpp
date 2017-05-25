#include <Arduino.h>

// Programme qui gère la pince du robot
// (ou pas)

#define LED 22

void ouvrirPince() {
    digitalWrite(LED, HIGH);
    delay(1000);
    return;
}

void fermerPince() {
    digitalWrite(LED, LOW);
    delay(1000);
    return;
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

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
    ouvrirPince();
}

void loop() {
}
