#include "heltec.h"
#include <VescUart.h>
VescUart UART;

String versionnr = "rcv_0.705";

      bool  recv;
      byte  packet[10];
      float lastrecvduty;
      float accel;
      float calcduty;
      float sendduty;
      float isduty;

      long  lastrecvtime;
      long  rampbetweentwovalues = 500;
      long  timebetweensendduty  =   10;
      long  timeout      = 1000;

      bool  serialenable=true;

      long lastvesc;
      long barposition = 0;

      byte channel =  0;
      byte buzzer  = 12;

void clearpacket()
    {
    for(int i=0; i<10; i++)
    {packet[i] =0;}
    }

void setup() {
  ledcSetup(channel, 500, 8);
  ledcAttachPin(buzzer, channel);
  
  Heltec.begin(true /*Display*/, true /*LoRa*/, false /*Serial*/, true /*PABOOST*/, 433E6 /*BAND*/);
  Serial.begin(115200);
  Serial1.begin(115200,SERIAL_8N1,23,17); //Kommunikation mit rewinch (23=RX (gelb), 17=TX (grün))
  UART.setSerialPort(&Serial1);
  delay(500);
  
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  delay(500);
    
  //LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.onReceive(onReceive);    // register the receive callback
  LoRa.receive();               // put the radio into receive mode
  
  Heltec.display->setFont(ArialMT_Plain_16);
  
  Heltec.display->drawString(10,32,"hello");
  Heltec.display->display();  
  delay(1000);
  
  Heltec.display->drawString(20,48,versionnr);
  Heltec.display->display();  
  
  delay(1000);
  Heltec.display->setFont(ArialMT_Plain_10); 
}


void loop() {
    if (lastvesc + 1000 < millis()) updatevesc();
    if(recv) {
          
          serialenable = true;
          recv = false;
          int intduty  = 0;
          if(packet[0] == 'd') intduty = packet[1]-100;
          Serial.print("int=");
          Serial.print(intduty);
          Serial.println();
          lastrecvduty = (float) intduty;
          if(lastrecvduty <= isduty + 0.1 && lastrecvduty >= isduty - 0.1) accel = 0.0;
          else {
            accel = (lastrecvduty - isduty) / rampbetweentwovalues * timebetweensendduty;
            if      (accel > -0.02 && accel < 0.00 ) accel = -0.02;
            else if (accel <  0.02 && accel > 0.00 ) accel =  0.02;
          }
        }
    
        
    if      (millis() > lastrecvtime + timeout) { sendduty = 0; }
    else {
          if(lastrecvduty <= isduty + 0.1 && lastrecvduty >= isduty - 0.1) sendduty = lastrecvduty;
          else    {
            calcduty = isduty + accel;
            if      (accel > -0.01 && isduty <= calcduty+0.1) sendduty = calcduty;
            else if (accel <  0.01 && isduty >= calcduty-0.1) sendduty = calcduty;
            else    sendduty = isduty;
          }
                    
        }
        
    if (barposition > 0) {sendduty = 0; }
      
    if(serialenable) {
        //Serial.print(millis());
        Serial.print("sendduty="); 
        Serial.print(sendduty); 
        
        UART.setDutyALL(sendduty/100.0);
        
        int freq = 500 + sendduty*4;
        Serial.print("freq="); 
        Serial.print(freq); 
        ledcWriteTone(channel, freq);
        
        if ( sendduty == 0 ) {
          ledcWrite(0, 0);
          serialenable = false;
          }

        Serial.println();
    }
              
    isduty = sendduty;
    delay(timebetweensendduty);
}   //end loop

void onReceive(int packetSize)
{
  recv = true;

  lastrecvtime=millis();
  clearpacket();
  //Serial.print("packet=");
  for (int i = 0; i < packetSize; i++)  // read packet
  {
    packet[i] += (char) LoRa.read();
    //Serial.print(packet[i]);
  }
  
}   //end onReceive


bool updatevesc() {
  
  /** Call the function getVescValues() to acquire data from VESC */
  lastvesc = millis();
  bool result;
  if ( UART.getVescValues() ) {
    barposition = UART.data.tachometer;
    Serial.println(UART.data.dutyCycleNow);
    result = 1;
  }
  
  Heltec.display->clear_firstline();  
  Heltec.display->drawString(10,0,String(sendduty));
  Heltec.display->drawString(70,0,String(barposition));
  Heltec.display->display();
  if (result) {return 1;}
  else  { return 0; }
}   //end updatevesc
