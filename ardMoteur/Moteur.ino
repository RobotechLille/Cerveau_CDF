#define PI 3.14159265359

//1 tick correspond à conversion cm
float conversion = (PI*8)/1024;

int long consigne_G;
int long consigne_D;
int cmd;
boolean received = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Serial.write(97);
  // delay(1000);
  if(received)
   {
      Serial.println(received);
      received = false;
   }
}

void floatToStr(float f, char r[8]) {
    char s[255];
    sprintf(s, "%8f", f);
    int i;
    for (i = 0; i < 8; i++) {
        r[i] = s[i];
    }
}

/* Fonction de lecture d'un float sur la liaison série */
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
  // char retour[8];
  // floatToStr(a, retour);
  char retour[8] = "123.4567";
  int i;
  for (i = 0; i < 8; i++) {
    Serial.write(retour[i]);
    delay(5);
  }
}

void serialEvent() {
  while(Serial.available()) 
  {
    cmd = (int)Serial.read();
    delay(1);
    float inFloat = Serialreadfloat();
    
    switch (cmd) {
      case 0 :
        //On recoit alors des centimetres qu'on convertit en ticks
        consigne_G = inFloat * conversion;
        consigne_D = consigne_G;
        break;
      case 1 :
        //On recoit alors des degres qu'on convertit en ticks
        consigne_G;
        consigne_D = consigne_G;
        break;
      default :
        consigne_G = 0;
        consigne_D = 0;
        break;
    }
    // DEBUG
    inFloat++;

    Serial.write(cmd);
    delay(1000);
    Serialsendfloat(inFloat);
  }
}


