//NOT READY!!

#include "heltec.h"

#define PINUP  2
#define PINDN  12
#define LEDLEFT 17
#define LEDRIGHT 22

byte TENSIONBUTTON;
byte RUNBUTTON;
bool lefthandmode;

//#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
//#define BUTTON_PIN_BITMASK 0x8004 // GPIOs 2 and 15
#define BUTTON_PIN_BITMASK 0x1004 // =2^12 + 2^2 in hex

String versionnr = "snd0.808";

RTC_DATA_ATTR int sleepspeed = 20;
byte displayspeed = sleepspeed;
byte tensionspeed = 2;

const unsigned long SLEEPTIMER = 20000;

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool changed;
};

Button button1 = {PINUP, 0, false};

unsigned long lastisr;
unsigned long pressedtime;
unsigned long releasedtime;

bool buttonishold = false;
bool increaseinnextloop;

bool previousstateUP;
bool previousstateDN;
unsigned long timeYELhigh;
unsigned long timeREDhigh;
unsigned long timeYELlow;
unsigned long timeREDlow;

const int HOLDTIME = 800;
const int PRESSTIME = 600;

bool runningmode;
bool tensionmode;
byte sendzero;

void IRAM_ATTR isr() {
  if ((millis() > lastisr + 50)) {
    button1.numberKeyPresses += 1;
    lastisr = millis();
    button1.changed = true;
    buttonishold = !buttonishold;
    if (buttonishold) pressedtime = millis();
    else releasedtime = millis();
  }
}

void setup() {
  Heltec.begin(true /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, 433E6 /*BAND*/);
  //displayspeed = sleepspeed;
  Heltec.display->setFont(ArialMT_Plain_10);
  //pinMode(button1.PIN, INPUT);
  pinMode(PINUP, INPUT);
  pinMode(PINDN, INPUT);
  //attachInterrupt(button1.PIN, isr, CHANGE);
  Serial.println("setupdone..");

  LoRa.setTxPower(20, RF_PACONFIG_PASELECT_PABOOST);

  pinMode(LEDLEFT, OUTPUT);
  pinMode(LEDRIGHT, OUTPUT);
  delay(500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Please release Buttons");
  Heltec.display->display(); 
  while(digitalRead(PINUP) || digitalRead(PINDN)) {
    digitalWrite(LEDRIGHT, LOW);
    digitalWrite(LEDLEFT, HIGH);
    delay(70);
    digitalWrite(LEDLEFT, LOW);
    digitalWrite(LEDRIGHT, HIGH);
    delay(70);
  }
  digitalWrite(LEDLEFT, HIGH);
  digitalWrite(LEDRIGHT, HIGH);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Please select Tensionbutton");
  Heltec.display->display();
  delay(500);
  bool PINUPstate=false;
  bool PINDNstate=false;
  while(!PINUPstate && !PINDNstate) {
    if(digitalRead(PINUP)) {
      PINUPstate=true;
      TENSIONBUTTON = PINUP;
      lefthandmode = true;
    }
    else if (digitalRead(PINDN)) {
      PINDNstate=true;
      TENSIONBUTTON = PINDN;
      lefthandmode = false;
    }
  }
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Tensionbutton = ");
  Heltec.display->drawString(0,10, (TENSIONBUTTON == PINDN) ? "LEFT" : "RIGHT");
  Heltec.display->display();
  delay(1000);
  
  digitalWrite(LEDLEFT, HIGH);
  digitalWrite(LEDRIGHT, HIGH);
  Heltec.display->clear();
}

void loop() {

  Heltec.display->clear();
  bool inUP = digitalRead(PINUP);
  bool inDN = digitalRead(PINDN);

  Heltec.display->drawString(0, 2, "UP=");
  Heltec.display->drawString(25, 2, String(inUP));
  //Heltec.display->drawString(30, 2, String(buttonishold));
  Heltec.display->drawString(40, 2, "DN=");
  Heltec.display->drawString(65, 2, String(inDN));

  if (inUP && !previousstateUP)     //inUP from 0 -> 1
    timeYELhigh = millis();
  if (!inUP && previousstateUP) {   //inUP from 1 -> 0
    timeYELlow  = millis();
    if (timeYELhigh > timeYELlow - PRESSTIME && displayspeed < 40 && !runningmode) displayspeed++;
  }

  if (!inUP && buttonishold) {
    Heltec.display->drawString(10,  20, " ISR didnt catch: ");
    Heltec.display->drawString(10,  30, "PINUP falling edge");
    buttonishold = false;
    button1.changed = false;
    Heltec.display->drawString(10,  40, "buttonishold -> 0");
    Heltec.display->drawString(10,  50, "program hold for 2s");
    Heltec.display->display();
    //delay(2000);  //removed, delay is good for finding program bugs, but bad whilst riding.
    Heltec.display->clear();
  }

  if (button1.changed == true) {
    if (releasedtime > pressedtime && releasedtime < pressedtime + PRESSTIME && displayspeed < 40) {
      displayspeed++;
      
    }
    button1.changed = false;
  }

  if (inDN && !previousstateDN)    //inDWN from 0 -> 1
    timeREDhigh = millis();
  if (!inDN && previousstateDN) {  //inDWN from 1 -> 0
    timeREDlow  = millis();
    if (timeREDhigh > timeREDlow - PRESSTIME && displayspeed > 0) displayspeed--;
  }

  if (timeYELhigh > timeYELlow && timeYELhigh + HOLDTIME < millis()) {
    if (runningmode) {
      if (displayspeed > 0) displayspeed --;
    }
    else tensionmode = true;
  }

  if (timeREDhigh > timeREDlow && timeREDhigh + HOLDTIME < millis()) {
    if (!runningmode)
      tensionmode = false;
    runningmode = true;
    attachInterrupt(button1.PIN, isr, CHANGE);
  }

  if (timeREDlow >= timeREDhigh && timeYELlow >= timeYELhigh) {
    long timeuntilsleep = ((timeREDlow > timeYELlow) ? timeREDlow : timeYELlow) + SLEEPTIMER - millis();
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



  if (runningmode && timeREDlow > timeREDhigh /*&& millis() > timeREDlow + 500*/) { //can have a delay before sending 0
    detachInterrupt(button1.PIN);
    runningmode = false;
    sendzero = 3;
  }
  if (tensionmode && timeYELlow > timeYELhigh /*&& millis() > timeYELlow + 500*/) { //can have a delay before sending 0
    tensionmode = false;
    sendzero = 3;
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

  if (runningmode || tensionmode || sendzero) {
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

  if (!runningmode && !tensionmode) delay(50);

  previousstateUP = inUP;
  previousstateDN = inDN;

}

void deepsleepstart() {
  //put fall asleep routine here
  sleepspeed = displayspeed;

  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(64, 15, "Good Night");
  Heltec.display->display();
  delay(2000);

  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_12,HIGH);
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  delay(100);

  LoRa.end();
  LoRa.sleep();
  delay(100);
  pinMode(5, INPUT);

  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
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
    if (pressedtime > releasedtime && pressedtime + 500 < millis() && increaseinnextloop && inDN) {
      if (displayspeed < 40) displayspeed += 2;
      Serial.printf("displayspeed: %u \n", displayspeed);
      Serial.printf("Button 1 changed %u times\n", button1.numberKeyPresses);
      //delay(200);
    }


    if (pressedtime > releasedtime) if (inDN) increaseinnextloop = true;
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
