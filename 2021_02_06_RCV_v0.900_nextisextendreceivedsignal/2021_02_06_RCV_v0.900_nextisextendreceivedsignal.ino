
#include "heltec.h"
#include "rewinchlogo.h"
#include <VescUart.h>
VescUart UART;

#define rampupmillis   4000
#define rampdownmillis 1000
#define timeoutmillis  1500
#define maxspeedkmh      40

String versionnr = "rcv0.900";

byte packet[10];

volatile bool recv;
float recvduty;
float prevrecvduty;
float rampbegspeed;
float rampendspeed;
float vescduty;
long recvtime;
long timeout = 0;
long rampbegmillis;
long rampendmillis;

long lastvescmillis;
long barposition = 0;
bool sendzero = false;

void setup() {

  Heltec.begin(true /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, 433E6 /*BAND*/);
  Serial1.begin(115200, SERIAL_8N1, 23, 17); //Kommunikation mit rewinch (23=RX (gelb), 17=TX (grün))
  UART.setSerialPort(&Serial1);
  delay(500);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  delay(500);

  LoRa.onReceive(onReceive);    // register the receive callback
  LoRa.receive();               // put the radio into receive mode

  Heltec.display->drawXbm(0, 0, logo_width, logo_height, rewinchlogo);
  Heltec.display->display();
  delay(500);
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(60, 45, versionnr);
  Heltec.display->display();
  delay(1000);
}


void loop() {
  delay(10);
  //if (lastvescmillis + 500 < millis()) updatevesc();
  delay(15);

  Heltec.display->clear();
  
  float printduty = -1.0;
  if (recv) {
    recv = false;
    byte byteduty  = 0;
    if (packet[0] == 'd') {
      byteduty = packet[1];
      timeout = recvtime + timeoutmillis;
    }
    recvduty = (float) byteduty;
    
    if (recvduty != prevrecvduty) {
      rampbegmillis = millis(); //previously: recvtime - but then there is no 0.00 being sended
      rampendmillis = rampbegmillis + (abs(recvduty - vescduty) / maxspeedkmh) * (recvduty > prevrecvduty ? rampupmillis : rampdownmillis);
      rampbegspeed = vescduty;
      rampendspeed = recvduty;
    }

    prevrecvduty = recvduty;
    printduty = recvduty;
  }
  
  if(!digitalRead(0)) {
    Heltec.display->clear();
    Heltec.display->drawString(10, 13, String(0.05));
    Heltec.display->drawString(0, 36, "!!rolling in!!");
    Heltec.display->display();
    while(!digitalRead(0)) { UART.setDutyALL(0.10); delay(50);}
    Heltec.display->clear();
    Heltec.display->drawString(10, 13, String(0.10));
    Heltec.display->drawString(65, 36, "!stop!"); 
    Heltec.display->display();
    UART.setDutyALL(0.00);
    delay(100);
  }
    

  if (millis() > timeout && vescduty != 0.0) {
    prevrecvduty = 0.0;
    timeout = millis() + rampdownmillis + 200;
    rampendspeed = 0.0;
    rampbegspeed = vescduty;
    rampbegmillis = millis();
    rampendmillis = rampbegmillis + (abs(vescduty) / maxspeedkmh) * rampdownmillis;
  }
  if (millis() < timeout) {
    if (millis() < rampendmillis) {
      vescduty = mapfloat (millis(), rampbegmillis, rampendmillis, rampbegspeed, rampendspeed);
    }
    else vescduty = rampendspeed;

    Serial.print(printduty);
    Serial.print(" ");
    Serial.println(vescduty);
    UART.setDutyALL(vescduty/40.0);
  }
  
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(10, 13, String(vescduty));
  Heltec.display->display();
  
}   //end loop


void onReceive(int packetSize)
{
  recv = true;
  recvtime = millis();
  for (int i = 0; i < 10; i++) {
    packet[i] = 0;   //clearpacket
  }
  //Serial.print("packet=");
  for (int i = 0; i < packetSize; i++)  // read packet
  {
    packet[i] += (char) LoRa.read();
    //Serial.print(packet[i]);
  }
}   //end onReceive


bool updatevesc() {
  // Call the function getVescValues() to acquire data from VESC
  lastvescmillis = millis();
  bool result = 0;
  if ( UART.getVescValues(0) ) {
    barposition = UART.data.tachometer;
    //Serial.println(UART.data.dutyCycleNow);
    result = 1;
  }
  return result;
}   //end updatevesc      */

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
