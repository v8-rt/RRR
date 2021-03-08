//NOT READY!!

#include "heltec.h"
#include "rewinchlogo.h"
#include "driver/rtc_io.h"

#define PINLFT  2
#define PINRGT 12
#define LEDLFT 17
#define LEDRGT 22

String versionnr = "S_0.913";

RTC_DATA_ATTR int sleepspeed = 20; //km/h
RTC_DATA_ATTR bool lefthandmode = false;
RTC_DATA_ATTR bool testmode2 = false;
RTC_DATA_ATTR int freq = 433;
RTC_DATA_ATTR unsigned long SLEEPTIMER = 30000;

byte displayspeed = sleepspeed;
byte tensionspeed = 5; //km/h

bool sendingLFT;
bool sendingRGT;
bool previousinLFT;
bool previousinRGT;
unsigned long timeLFTrise;
unsigned long timeRGTrise;
unsigned long timeLFTfall;
unsigned long timeRGTfall;

const int HOLDTIME = 600;
const int TIMEBEFORESENDINGZERO = 300;

bool runningmode;
bool tensionmode;
byte sendzero;

bool sendingflag;
byte loraspeed;

void setup() {
  Heltec.begin(true /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, freq*1000000 /*BAND*/);
  //displayspeed = sleepspeed;
  Heltec.display->clear();
  Heltec.display->drawXbm(0, 0, logo_width, logo_height, rewinchlogo);
  Heltec.display->display();
  delay(2000);

  Heltec.display->setFont(ArialMT_Plain_10);
  //pinMode(button1.PIN, INPUT);
  pinMode(PINLFT, INPUT_PULLUP);
  pinMode(PINRGT, INPUT_PULLUP);
  //attachInterrupt(button1.PIN, isr, CHANGE);
  Serial.println("setupdone..");
  

/*
  LoRa.setSpreadingFactor(8);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(4);
  LoRa.setSyncWord(0x12);   //0x34
  LoRa.setPreambleLength(8);
*/
  LoRa.setTxPower(20, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.setSpreadingFactor(8);
  
  pinMode(LEDLFT, OUTPUT);
  pinMode(LEDRGT, OUTPUT);
  delay(500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Please release Buttons");
  Heltec.display->display();
  while (!digitalRead(PINLFT) || !digitalRead(PINRGT)) {
    digitalWrite(LEDRGT, LOW);
    digitalWrite(LEDLFT, HIGH);
    delay(70);
    digitalWrite(LEDLFT, LOW);
    digitalWrite(LEDRGT, HIGH);
    delay(70);
  }
  digitalWrite(LEDLFT, HIGH);
  digitalWrite(LEDRGT, HIGH);


}

void loop() {
  bool inLFT = !digitalRead(PINLFT);
  bool inRGT = !digitalRead(PINRGT);

  if (inLFT && !previousinLFT) {
    timeLFTrise = millis();
  }

  if (inRGT && !previousinRGT) {
    timeRGTrise = millis();
  }
  
  if (inLFT && inRGT && millis() < timeLFTrise + HOLDTIME && millis() < timeRGTrise + HOLDTIME && !sendingLFT && !sendingRGT) {
    menu();
    timeLFTfall = millis();
    timeRGTfall = millis();
    inLFT = false;
    inRGT = false;
    previousinLFT = false;
    previousinRGT = false;
  }

  if (!inLFT && previousinLFT) {
    timeLFTfall = millis();
    if (timeLFTfall < timeLFTrise + HOLDTIME && displayspeed >  0 && !sendingLFT) {
      displayspeed--;
    }
  }
  
  if (!inRGT && previousinRGT) {
    timeRGTfall = millis();
    if (timeRGTfall < timeRGTrise + HOLDTIME && displayspeed < 40 && !sendingRGT) {
      displayspeed++;
    }
  }

  if (inLFT && millis() > timeLFTrise + HOLDTIME) {
    if(!inRGT) sendingLFT = true;
    else {
      if(displayspeed < 40) displayspeed++;
      timeLFTrise= millis();
    }
  }

  if (inRGT && millis() > timeRGTrise + HOLDTIME && !inLFT) {
    sendingRGT = true;
  }

  if (sendingLFT && !inLFT && millis() > timeLFTfall + TIMEBEFORESENDINGZERO) {
    sendingLFT = false;
    if (!sendingRGT) {
      sendingflag = true;
      loraspeed = 0;
    }
  }

  if (sendingRGT && !inRGT && millis() > timeRGTfall + TIMEBEFORESENDINGZERO) {
    sendingRGT = false;
    if (!sendingLFT) {
      sendingflag = true;
      loraspeed = 0;
    }
  }

  Heltec.display->clear();

  if (sendingLFT && (lefthandmode ? true : !sendingRGT)) {
    sendingflag = true;
    loraspeed = lefthandmode ?  displayspeed : tensionspeed;
    Heltec.display->drawString(0, 53, String("sendingLFT: " + String(lefthandmode ? displayspeed : tensionspeed) + " km/h"));
  }

  if (sendingRGT && (lefthandmode ? !sendingLFT : true)) {
    sendingflag = true;
    loraspeed = lefthandmode ? tensionspeed : displayspeed;
    Heltec.display->drawString(0, 53, String("sendingRGT:  " + String(lefthandmode ? tensionspeed : displayspeed) + " km/h"));
  }

  Heltec.display->drawXbm(0, 0, logomini_width, logomini_height, rewinchlogomini);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->setFont(Orbitron_Medium_48);
  Heltec.display->drawString(58,14,String(displayspeed));
  Heltec.display->setFont(Orbitron_Medium_10);
  Heltec.display->drawString(58, 2,"SPEED");
  Heltec.display->drawString(112, 28,"km/h");
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

  checktimetosleep();

  Heltec.display->display();

  previousinLFT = inLFT;
  previousinRGT = inRGT;

  if (sendingflag) sendlora();
  else delay(50);
}

void sendlora() {/*byte loraspeed*/
  LoRa.beginPacket();
  LoRa.print("d");
  LoRa.write(loraspeed);
  LoRa.endPacket();
  sendingflag = false;
}

void checktimetosleep() {
  if (timeRGTfall >= timeRGTrise && timeLFTfall >= timeLFTrise) {
    long timeuntilsleep = ((timeRGTfall > timeLFTfall) ? timeRGTfall : timeLFTfall) + SLEEPTIMER - millis();
    Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
    Heltec.display->drawString(128,  2, String(timeuntilsleep / 1000));
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawString( 90,  2, "zZ:");
    if (timeuntilsleep < 0) {
      deepsleepstart();
    }
  }
}

void menu() {
  Heltec.display->clear();
  Heltec.display->drawString(40, 2, "---MENU---");
  Heltec.display->drawString(20, 12, "lefthandmode");
  Heltec.display->drawString(100, 12, lefthandmode ? "On" : "Off");
  Heltec.display->drawString(20, 22, "testmode2");
  Heltec.display->drawString(100, 22, testmode2 ? "On" : "Off");
  Heltec.display->drawString(20, 32, "SLEEPTIMER");
  Heltec.display->drawString(100, 32, String(SLEEPTIMER / 1000));
  Heltec.display->drawString(20, 42, "frequency");
  Heltec.display->drawString(100, 42, String(freq));
  Heltec.display->drawString(20, 52, "Exit");
  Heltec.display->display();
  delay(500);
  while (!digitalRead(PINLFT) || !digitalRead(PINRGT)) {
    digitalWrite(LEDRGT, LOW); digitalWrite(LEDLFT, HIGH); delay(70);
    digitalWrite(LEDLFT, LOW); digitalWrite(LEDRGT, HIGH); delay(70);
  }
  bool exitmenu = false;
  byte menupos = 0;
  Heltec.display->drawString(5, 12 + menupos * 10, ">");
  Heltec.display->display();
  while (!exitmenu) {
    if (!digitalRead(PINLFT)) {
      menupos++;
      if (menupos > 4) menupos = 0;
      Heltec.display->setColor(BLACK);
      Heltec.display->fillRect(5, 12, 10, 50);
      Heltec.display->setColor(WHITE);
      Heltec.display->drawString(5, 12 + menupos * 10, ">");
      Heltec.display->display();
      delay(300);
    }
    if (!digitalRead(PINRGT)) {
      unsigned long setpressed = millis();
      bool menuchangeval = true;
      while (menuchangeval) {
        delay(100);
        if (digitalRead(PINRGT) || millis() > setpressed + 500) {
          switch (menupos) {
            case 0: lefthandmode = !lefthandmode;
              Heltec.display->setColor(BLACK);
              Heltec.display->fillRect(100, 12, 100, 10);
              Heltec.display->setColor(WHITE);
              Heltec.display->drawString(100, 12, lefthandmode ? "On" : "Off");
              Heltec.display->display();
              break;
            case 1: testmode2 = !testmode2;
              Heltec.display->setColor(BLACK);
              Heltec.display->fillRect(100, 22, 100, 10);
              Heltec.display->setColor(WHITE);
              Heltec.display->drawString(100, 22, testmode2 ? "On" : "Off");
              Heltec.display->display();
              LoRa.beginPacket();
              LoRa.print("L");
              LoRa.write(testmode2);
              LoRa.endPacket();
              break;
            case 2:
              (millis() > setpressed + 500) ? SLEEPTIMER += 5000 : SLEEPTIMER -= 5000;
              Heltec.display->setColor(BLACK);
              Heltec.display->fillRect(100, 32, 100, 10);
              Heltec.display->setColor(WHITE);
              Heltec.display->drawString(100, 32, String(SLEEPTIMER / 1000));
              Heltec.display->display();
              break;
            case 3:
              freq == 433 ? freq = 868 : freq = 433;
              Heltec.display->setColor(BLACK);
              Heltec.display->fillRect(100, 42, 100, 10);
              Heltec.display->setColor(WHITE);
              Heltec.display->drawString(100, 42, freq == 433 ? "433" : "868");
              Heltec.display->display();
              LoRa.setFrequency(freq*1000000);
              break;
            case 4: exitmenu = true; break;
          }
          while(!digitalRead(PINRGT)){;}
          menuchangeval=false;
          delay(100);
        }
      }
    }
  }


  while (!digitalRead(PINLFT) || !digitalRead(PINRGT)) {
    digitalWrite(LEDRGT, LOW); digitalWrite(LEDLFT, HIGH); delay(70);
    digitalWrite(LEDLFT, LOW); digitalWrite(LEDRGT, HIGH); delay(70);
  }
  digitalWrite(LEDLFT, HIGH);
  digitalWrite(LEDRGT, HIGH);
}







/*

  void notinuse() {
  Heltec.display->drawString(0, 2, "LFT=");
  Heltec.display->drawString(30, 2, String(inLFT));
  Heltec.display->drawString(45, 2, "RGT=");
  Heltec.display->drawString(75, 2, String(inRGT));

  if (inLFT && !previousstateLFT) {   //inLFT from 0 -> 1
    timeLFThigh = millis();
    skipgap = false;
  }
  if (!inLFT && previousstateLFT) {  //inLFT from 1 -> 0
    timeLFTlow  = millis();
    skipgap = true;
    if (timeLFTlow < timeLFThigh + HOLDTIME && displayspeed > 0) displayspeed--;
  }

  if (inRGT && !previousstateRGT)  {  //inRGT from 0 -> 1
    timeRGThigh = millis();
    skipgap = false;
  }
  if (!inRGT && previousstateRGT) {  //inRGT from 1 -> 0
    timeRGTlow  = millis();
    skipgap = true;
    if (timeRGTlow < timeRGThigh + HOLDTIME && displayspeed < 40) displayspeed++;
  }

  if (timeLFThigh > timeLFTlow && millis() > timeLFThigh + HOLDTIME) {
    if (lefthandmode) runningmode = true;
    else tensionmode = true;
  }

  if (timeRGThigh > timeRGTlow && timeRGThigh + HOLDTIME < millis()) {
    if (lefthandmode) tensionmode = true;
    else runningmode = true;
  }

  if (timeRGTlow >= timeRGThigh && timeLFTlow >= timeLFThigh) {
    long timeuntilsleep = ((timeRGTlow > timeLFTlow) ? timeRGTlow : timeLFTlow) + SLEEPTIMER - millis();
    Heltec.display->drawString(106,  2, String(timeuntilsleep / 1000));
    if (timeuntilsleep < 0) {
      deepsleepstart();
    }
  }

  Heltec.display->drawString( 0, 28, "speed=");
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(44, 20, String(displayspeed));
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(80, 28, "km/h");


  if (runningmode && skipgap && millis() > (lefthandmode ? timeLFTlow : timeRGTlow) + 500) { //can have a delay before sending 0
    Serial.println("stoprun");
    runningmode = false;
    sendzero = 3;
    skipgap = false;
  }
  if (tensionmode && skipgap && millis() > (lefthandmode ? timeRGTlow : timeLFTlow) + 500) { //can have a delay before sending 0
    Serial.println("stopten");
    tensionmode = false;
    sendzero = 3;
    skipgap = false;
  }


  if (runningmode) {
    Heltec.display->drawString(0, 50, "running...");
    Heltec.display->drawString(55, 50, String(displayspeed));
    Heltec.display->drawString(80, 50, "km/h");
  }
  if (tensionmode) {
    Heltec.display->drawString(0, 50, "tensioning..");
    Heltec.display->drawString(55, 50, String(tensionspeed));
    Heltec.display->drawString(80, 50, "km/h");
  }
  if (sendzero) {
    Heltec.display->drawString(0, 50, "breaking..");
    Heltec.display->drawString(55, 50, String(0));
    Heltec.display->drawString(80, 50, "km/h");
  }

  Heltec.display->display();
  Serial.printf("TLH %u TLL %u TRH %u TRL %u \n", timeLFThigh, timeLFTlow, timeRGThigh, timeRGTlow);

  if (runningmode || tensionmode || sendzero) {
    if (!skipgap) {       //während skipgap kein senden!
      Serial.println("SENDING");
      LoRa.beginPacket();
      LoRa.print("d");
      if (runningmode)      LoRa.write(displayspeed);
      else if (tensionmode) LoRa.write(tensionspeed);
      else if (sendzero) {
        LoRa.write(0);
        sendzero--;
      }
      LoRa.endPacket();
    }
  }

  if (!runningmode && !tensionmode) delay(50);

  previousstateLFT = inLFT;
  previousstateRGT = inRGT;
  }

*/



void deepsleepstart() {  //put fall asleep routine here
  sleepspeed = displayspeed;

  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(64, 15, "Good Night");
  Heltec.display->display();
  //LED FadeOut:
  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcAttachPin(LEDLFT, 0);
  ledcAttachPin(LEDRGT, 1);
  for (byte i = 200; i > 100; i--) {
    ledcWrite(0, i);
    ledcWrite(1, i);
    delay(10);
  }
  for (byte i = 100; i > 0; i--) {
    ledcWrite(0, i);
    ledcWrite(1, i);
    delay(25);
  }
  ledcDetachPin(LEDLFT);
  pinMode(LEDLFT, INPUT); //added
  ledcDetachPin(LEDRGT);
  pinMode(LEDRGT, INPUT); //added

  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_12,HIGH);
  //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  rtc_gpio_pullup_en(GPIO_NUM_2);
  rtc_gpio_pulldown_dis(GPIO_NUM_2);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, 0);

  delay(100);

  LoRa.end();
  LoRa.sleep();
  delay(100);
  pinMode(5, INPUT);

  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  //pinMode(17, INPUT);


  pinMode(18, INPUT);
  pinMode(19, INPUT);

  pinMode(26, INPUT);
  pinMode(27, INPUT);
  delay(100);
  Serial.println("Going to sleep now");
  delay(2);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}







//OUT OF ORDER

/*
    if (pressedtime > releasedtime && pressedtime + 500 < millis() && increaseinnextloop && inRGT) {
      if (displayspeed < 40) displayspeed += 2;
      Serial.printf("displayspeed: %u \n", displayspeed);
      Serial.printf("Button 1 changed %u times\n", button1.numberKeyPresses);
      //delay(200);
    }


    if (pressedtime > releasedtime) if (inRGT) increaseinnextloop = true;
      else if (button1.changed) {
        button1.changed = false;
        Serial.printf("Button 1 changed %u times\n", button1.numberKeyPresses);
        if (releasedtime < pressedtime + 500) {
          if (displayspeed > 0)  displayspeed -= 2;
          Serial.printf("buttonishold: %u \n", buttonishold);
        }
        Serial.printf("displayspeed: %u ", displayspeed);

      }
*/
/*
  if ( button1.numberKeyPresses & 0x01 ) {
    Serial.printf("H %u ", button1.numberKeyPresses);
    delay(100);
  }
*/
