#include "M5StickC.h"
#include "fonts.h"
TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

RTC_TimeTypeDef TimeStruct;

float vbat,vin,temp;
int charge;
String hr,mi,se;
int deb=0; int deb2=0;

#define color 0x01EA
#define color2 ORANGE

String cc[4]={"100 mAh","280 mAh","450 mAh","700 mAh"};
byte values[4]={0,2,4,7};
byte chosen=1;

/*
#define CURRENT_100MA (0b0000)
#define CURRENT_190MA (0b0001)
#define CURRENT_280MA (0b0010)
#define CURRENT_360MA (0b0011)
#define CURRENT_450MA (0b0100)
#define CURRENT_550MA (0b0101)
#define CURRENT_630MA (0b0110)
#define CURRENT_700MA (0b0111)
*/

byte brightness[5] ={20,40,60,80,90};
byte chosenB=1;

void setup(){
  pinMode(37,INPUT_PULLUP);
  pinMode(39,INPUT_PULLUP);
  M5.begin();
  spr.createSprite(80,160);
  TimeStruct.Hours   = 00;
  TimeStruct.Minutes = 00;
  TimeStruct.Seconds = 00;
  M5.Rtc.SetTime(&TimeStruct);

 M5.Axp.ScreenBreath(brightness[chosenB]);
 M5.Axp.SetChargeCurrent(values[chosen]);
}

void draw()
{
  spr.fillSprite(TFT_BLACK);
 
  for(int i=0;i<5;i++)
  spr.fillRect(50+(i*6),1,4,12,color);

  for(int i=0;i<chosenB+1;i++)
  spr.fillRect(50+(i*6),1,4,12,color2);

  spr.setTextDatum(0);
  spr.setTextFont(1);
  spr.setTextColor(WHITE,BLACK); //0083
  spr.drawString("BAT",2,1,2);
  spr.drawString("SET CURRENT",2,85);
  //spr.setTextColor(WHITE,BLACK);
  spr.setFreeFont(&DSEG7_Classic_Bold_17);
  spr.drawString(String(vbat),1,20);
  spr.drawString("V",50,15,2);


  spr.setFreeFont(&DSEG7_Classic_Bold_12);
  spr.drawString(hr+":"+mi,1,144);
  spr.setFreeFont(&DSEG7_Classic_Bold_12);
  spr.drawString(se,52,141);
  
  spr.setTextFont(1);
  for(int i=0;i<4;i++)
  spr.drawString(cc[i],36,96+(i*10));
  
  spr.fillCircle(31,99+(chosen*10),2,TFT_RED);
  
  spr.drawFastHLine(1, 95, 25, color2);
  spr.drawFastVLine(25, 95, 42, color2);
  spr.drawFastVLine(26, 95, 42, color2);
  spr.drawFastHLine(25, 137, 54, color2);

  spr.setTextFont(0);
  spr.setTextColor(0x4C77,BLACK);
  spr.drawString("TIME",0,131);
   
  spr.drawString("TEMP",0,98);
  spr.drawString(String(temp,1),0,108);
  
 // spr.setTextDatum(4);
  spr.fillRoundRect(0, 42, 38,39,4,color);
  spr.fillRoundRect(41, 42, 38,39,4,color);
  
  spr.setTextFont(0);
  spr.setTextColor(WHITE,color);
  spr.drawString("USB V",4,45);

  spr.drawString("CHR I",45,45);
  spr.setFreeFont(&DSEG7_Classic_Bold_12);
  spr.drawString(String(vin),3,60);
  spr.drawString(String(charge),50,60);

  spr.pushSprite(0,0); 
}

void loop() {

   if(digitalRead(37)==0)
   {
   if(deb==0) {deb=1; chosen++; if(chosen==4) chosen=0;  M5.Axp.SetChargeCurrent(values[chosen]);}
   }else deb=0;

   if(digitalRead(39)==0)
   {
   if(deb2==0) {deb2=1; chosenB++; if(chosenB==5) chosenB=0;  M5.Axp.ScreenBreath(brightness[chosenB]);}
   }else deb2=0;

   vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
   charge = M5.Axp.GetIchargeData() / 2;
   vin=M5.Axp.GetVBusVoltage();
   temp = M5.Axp.GetTempData()*0.1-144.7;

  M5.Rtc.GetTime(&TimeStruct);
 
  if(TimeStruct.Hours<10) hr="0"+String(TimeStruct.Hours); else hr=String(TimeStruct.Hours);
  if(TimeStruct.Minutes<10) mi="0"+String(TimeStruct.Minutes); else mi=String(TimeStruct.Minutes);
  if(TimeStruct.Seconds<10) se="0"+String(TimeStruct.Seconds); else se=String(TimeStruct.Seconds);

  draw();
  
}
