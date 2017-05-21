#include <Arduino.h>
#include <avr/io.h>

// Programme qui gère la pince du robot
// (ou pas)

void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRG = 0xFF;
    PORTC = 0x00;
    Serial.begin(9600);
}

void ouvrirPince() {
    PORTG = 0xFF;
    return;
}

void fermerPince() {
    PORTG = 0x00;
    return;
}


void serialEvent() {
    char command = Serial.read();
    PORTA = command;
    switch (command) {
        case 0x00:
            ouvrirPince();
            break;
        case 0x01:
            fermerPince();
            break;
    }
    // Serial.write(0);
}

void loop() {
}
