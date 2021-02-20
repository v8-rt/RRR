#include "heltec.h"

void setup() {
  // put your setup code here, to run once:
  Heltec.begin(true /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, 868E6 /*BAND*/);
  
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
}

void loop() {
  // put your main code here, to run repeatedly:
  int num = random(40);
  Heltec.display->clear();
  Heltec.display->setFont(Orbitron_Medium_10);
  Heltec.display->drawString(58, 0,"SPEED");
  Heltec.display->drawString(112, 30,"km/h");
  Heltec.display->setFont(Orbitron_Medium_48);
  Heltec.display->drawString(58,15,String(num));
  Heltec.display->display();
  delay(3000);
}
