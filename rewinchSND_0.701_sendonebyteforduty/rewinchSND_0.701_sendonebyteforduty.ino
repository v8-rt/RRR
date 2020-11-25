//************REWINCH***************
#include "heltec.h"
#include "rewinchlogo.h"


RTC_DATA_ATTR int bootCount = 0;

String versionnr = "snd0.701";
unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

int maxspeed=50;
int rampup=2000;
int rampdown=1000;
int duty;

bool recvX;
byte safety;

int menuflag;

unsigned long lastsend;
const int SLEEPTIMER = 40000;

int percent;

void setup()
{
  Heltec.begin(true /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, 433E6 /*BAND*/);
  
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  Heltec.display->init();
  Heltec.display->flipScreenVertically(); 

  adcAttachPin(37);
  analogSetClockDiv(255);
  
  rewinchinit();
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.onReceive(onReceive);

  setmaxspeed();
  
  //delay(2000);
  if(recvX == HIGH)
  {
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(64,48,String(packet));
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->display();
  delay(2000);
  }
}

void loop()
{
  int readin = analogRead(39);
  int duty = mapreadval (readin);
  bool sendflag = 0;
  long timeuntilsleep;
//  uint16_t batterylevel;
    
  if (duty!=0.0) safety = 0;
  if (duty!=0.0 || safety<5) {sendflag = sendduty(duty); safety++; lastsend=millis();}

  
  
  if (safety==5) {
//    batterylevel  =  analogRead(13);
    timeuntilsleep = lastsend + SLEEPTIMER - millis();
    if(timeuntilsleep < 0) {
      deepsleepstart();
    } //end if > SLEEPTIMER

  }
  
  if(menuflag > 30) 
    runmenu();
    
  Heltec.display->clear();
  Heltec.display->drawXbm(0,0,logomini_width,logomini_height,rewinchlogomini);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(64, 0,String(readin));  //remove if want to show battery
  if (sendflag) {
    //Heltec.display->drawString(64, 0,String(readin));
    Heltec.display->drawXbm(108,0,sending_s_w,sending_s_h,sending_s);
    
  }
  else {
//    Heltec.display->drawString(64, 0,String(batterylevel));
    Heltec.display->drawString(106,  0, String(timeuntilsleep/1000));
    delay(100);
  }
  
  Heltec.display->setFont(DSEG14_Classic_Bold_50); //put bigfont here
  if(duty==-100) {
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawString(0,8,"-");
    Heltec.display->drawString(10,8,"100");
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  }
  else  Heltec.display->drawString(64,8,String(duty));
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->display();
}

void rewinchinit() {
  
  //pinMode(0, INPUT);  //???
  Heltec.display->clear();
  Heltec.display->drawXbm(0,0,logo_width,logo_height,rewinchlogo);    //show logo
  Heltec.display->display();
  delay (1000);
  
  int battval = 0;
  for ( int i = 0; i < 40; i++ )
    {
      battval += analogRead(37);
      delay(5);
    }
  battval = battval * 2.92 / 40;
  percent = map (battval, 3500, 4000, 0, 100);
  if (percent < 0)  percent = 0;
  else if (percent > 100)  percent = 100;
  
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(70, 10,String(battval));
  Heltec.display->drawString(70, 0,String(percent));
  Heltec.display->drawString(100, 0,"%");
  Heltec.display->drawString(60,45,versionnr);
  Heltec.display->display();
  delay(2000);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
}

void setmaxspeed() {
    Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  int readval;
  int holdon = 10;
  while(holdon > 0) {
    Heltec.display->clear();
    readval = analogRead(39);   //0....4096
    if (readval > 2800 && maxspeed < 100) maxspeed+=5; 
    if (readval > 0 && readval < 1000 && maxspeed > 5) maxspeed-=5;
    
    if (readval > 1300 && readval < 2000) holdon--;
    else holdon = 10;
    Heltec.display->drawString(64,0,"Set maxSpeed(%)");       //in 5er schritten
    Heltec.display->drawString(64,20,String(maxspeed));
    Heltec.display->drawString(110,40,String(holdon));
    Heltec.display->display();
    delay(200);
  }
  //maxspeed = readval;
  Heltec.display->clear();
  Heltec.display->drawString(64,0,"maxSpeed saved!");       //in 5er schritten
  Heltec.display->drawString(64,20,String(maxspeed));
  Heltec.display->display();
  delay(1000);
  Heltec.display->drawString(64,40,"release!!");
  Heltec.display->display();
  holdon = 10;
  while(analogRead(39) != 0) {;}      //wait until sensor released
  delay(50);
}

void setmaxandramps() {
  setmaxspeed();
  int readval;
  int holdon = 10;
  while(holdon > 0) {
    Heltec.display->clear();
    readval = analogRead(39);   //0....4096
    if (readval > 2800 && rampup<9999) rampup+=500; 
    if (rampup==10000) rampup=9999;
    if (readval > 0 && readval < 1000 && rampup > 500) rampup-=500;

    if (readval > 1300 && readval < 2000) holdon--;
    else holdon = 10;
    
    Heltec.display->drawString(64,0,"rampup(ms)");       //in 5er schritten
    Heltec.display->drawString(64,20,String(rampup));
    Heltec.display->drawString(110,40,String(holdon));
    Heltec.display->display();
    delay(200);
  }
  Heltec.display->clear();
  Heltec.display->drawString(64,0,"rampup saved!");       //in 5er schritten
  Heltec.display->drawString(64,20,String(rampup));
  Heltec.display->display();
  delay(1000);
  Heltec.display->drawString(64,40,"release!!");
  Heltec.display->display();
  holdon = 10;
  while(analogRead(39) != 0) {;}      //wait until sensor released
  delay(50);
  
  while(holdon > 0) {
    Heltec.display->clear();
    readval = analogRead(39);   //0....4096
    if (readval > 2800 && rampdown<5000) rampdown+=250;
    if (readval > 0 && readval < 1000 && rampdown > 250) rampdown-=250;

    if (readval > 1300 && readval < 2000) holdon--;
    else holdon = 10;
    
    Heltec.display->drawString(64,0,"rampdown(ms)");       //in 5er schritten
    Heltec.display->drawString(64,20,String(rampdown));
    Heltec.display->drawString(110,40,String(holdon));
    //Heltec.display->drawString(64,40,"press but0 to save");
    Heltec.display->display();
    delay(200);
  }
  Heltec.display->clear();
  Heltec.display->drawString(64,0,"rampdown saved!");       //in 5er schritten
  Heltec.display->drawString(64,20,String(rampdown));
  Heltec.display->display();
  delay(10);
  Heltec.display->drawString(64,40,"release!!");
  Heltec.display->display();
  holdon = 10;
  while(analogRead(39) != 0) {;}      //wait until sensor released
  delay(50);

  Heltec.display->clear();
  Heltec.display->drawString(64,0,"Setup complete :)");
  Heltec.display->drawString(42,16,"maxspeed:");
  Heltec.display->drawString(106,16,String(maxspeed));
  Heltec.display->drawString(42,32,"rampup:");
  Heltec.display->drawString(106,32,String(rampup));
  Heltec.display->drawString(42,48,"rampdown:");
  Heltec.display->drawString(106,48,String(rampdown));
  Heltec.display->display();

    sendramps();

  delay(2000);
  lastsend=millis();
}



int mapreadval(int readval) {
    
    if (readval == 0)
      {
      menuflag=0;
      readval=0;
      }
    else if (readval<1350) 
      {
      menuflag=0;
      readval = map( readval, 50, 1300, -maxspeed, 0);
      if (readval < -maxspeed) readval = -maxspeed;
      if (readval > -2)   readval = -2;
      }
    else if (readval>1650) 
      {
      menuflag=0;
      readval = map( readval, 1700, 3900, 0, maxspeed);
      if (readval < 2)    readval = 2;
      if (readval > maxspeed)  readval = maxspeed;
      }
    else if (readval>1350 && readval<1650)
      {
      menuflag++;
      readval=0;
      }
    return readval;
}


void sendramps() {
  Serial.println("packet start");
  LoRa.beginPacket();
  LoRa.print("ramps");
  if(rampup<1000) LoRa.print("0");
  LoRa.print(rampup);
  
  LoRa.print("x");
  if(rampdown<1000) LoRa.print("0");
  LoRa.print(rampdown);
  LoRa.print("x");
  if(maxspeed<10) LoRa.print("0");
  if(maxspeed<100) LoRa.print("0");  
  LoRa.print(maxspeed);
  LoRa.endPacket();
  Serial.println("packet sent");
  delay(20);
  LoRa.receive();
}

bool sendduty(int intsendduty) {
  Serial.print("packet start"); Serial.println(millis());
  LoRa.beginPacket();
  LoRa.print("d");
  byte byteduty = intsendduty+100;
  Serial.println(byteduty);
  LoRa.write(byteduty);
  LoRa.endPacket();
  Serial.print("packet sent"); Serial.println(millis());
  //delay(20);
  return 1;
}

void onReceive(int packetSize)
{

  recvX=HIGH;
  // received a packet
  Serial.print("Received packet '");
  // read packet
  packet ="";
  for (int i = 0; i < packetSize; i++)
  {
    packet += (char) LoRa.read();
    //Serial.print((char)LoRa.read());
  }
  Serial.print(packet);
  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
  //Heltec.display->drawString(100, 50, "O");               -> nicht zu viel in onReceive schreiben!!
  //Heltec.display->display();
}


void runmenu() {
  menuflag=0;
  bool sendposA=0, sendposB=0, sendAUTO=0;
  Heltec.display->clear();
  Heltec.display->drawString(0,0,"runmenu:");
  Heltec.display->drawString(70, 0,String(percent));
  Heltec.display->drawString(100, 0,"%");
  Heltec.display->drawString(20,24,"posA"); 
  Heltec.display->drawString(100,24,"posB"); 
  Heltec.display->drawString(64,48,"AUTO"); 
  Heltec.display->display();
  delay(2000);
  while(!sendposA && !sendposB && !sendAUTO) {
      int readval = analogRead(39);
      if ( readval > 0 && readval < 1000) sendposA=HIGH ;
      if ( readval > 1300 && readval < 1800) sendAUTO=HIGH ;      
      if ( readval > 2800) sendposB=HIGH ;
      delay(100);
    }
  
  Heltec.display->clear();
  delay(100);
  if(sendAUTO) {
    Heltec.display->drawString(64,20,"hold: AUTO");
    Heltec.display->drawString(64,40,"rel: max&ramps");
    }
  else Heltec.display->drawString(64,20,"send:");
  Heltec.display->display();
  
  if (sendposA) {
    LoRa.beginPacket();
    LoRa.print("posA");
    LoRa.endPacket();
    Heltec.display->drawString(64,40,"posA");
    sendposA = 0;
  }
  else if (sendposB) {
    LoRa.beginPacket();
    LoRa.print("posB");
    LoRa.endPacket();
    Heltec.display->drawString(64,40,"posB");
    sendposB = 0;
  }
  else if (sendAUTO) {
    delay(2000);
    Heltec.display->clear();
    if (analogRead(39) > 1000) 
      {
        LoRa.beginPacket();
        LoRa.print("AUTO");
        Heltec.display->drawString(64,40,"AUTO started");
        sendAUTO = 0;
        LoRa.endPacket();
      }
    else
      setmaxandramps();
    /**/
  }
  
  Heltec.display->display();
  delay(2000);
}

void deepsleepstart() {
//put fall asleep routine here
      Heltec.display->clear();
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
      Heltec.display->setFont(ArialMT_Plain_24);
      Heltec.display->drawString(64,15,"Good Night");
      Heltec.display->display();
      delay(3000);
      
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_39,HIGH);
      delay(100);
      
      LoRa.end();
      LoRa.sleep();
      delay(100);
      pinMode(5,INPUT);

      pinMode(14,INPUT);
      pinMode(15,INPUT);
      pinMode(16,INPUT);
      pinMode(17,INPUT);
      pinMode(18,INPUT);
      pinMode(19,INPUT);

      pinMode(26,INPUT);
      pinMode(27,INPUT);
      delay(100);
      Serial.println("Going to sleep now");
      delay(2);
      esp_deep_sleep_start();
      Serial.println("This will never be printed");
}
