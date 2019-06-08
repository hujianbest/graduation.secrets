/**
************************************************************
* @file         Gizwits + ArduinoUnoR3 Library 
* @brief        Datapoints handle , Gizwits Process
* @author       Gizwits
* @date         2017-08-01
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include <Gizwits.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LEDS 7

#define   KEY1              3
#define   KEY2              4
#define   KEY1_SHORT_PRESS  1
#define   KEY1_LONG_PRESS   2
#define   KEY2_SHORT_PRESS  4
#define   KEY2_LONG_PRESS   8
#define   NO_KEY            0
#define   KEY_LONG_TIMER    3

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS,PIN,NEO_GRB+NEO_KHZ800);
SoftwareSerial mySerial(A2, A3); // A2 -> RX, A3 -> TX
Gizwits myGizwits;

unsigned long varR_LED_R=0;  
unsigned long varR_LED_G=0; 
unsigned long varR_LED_B=0; 
unsigned long varR_LED_Color = 0;
bool varR_LED_OnOff = 0;
unsigned long Last_KeyTime = 0;

void setPixelColorAll(uint8_t r,uint8_t g,uint8_t b)
{
  for(int i=0;i<pixels.numPixels();i++)
  {
    pixels.setPixelColor(i,pixels.Color(r,g,b));
    pixels.setPixelColor(i,pixels.Color(r,g,b));
    pixels.show();
  }
}


unsigned long gokit_time_s(void)
{
  return millis() / 1000;
}

char gokit_key1down(void)
{
  unsigned long keep_time = 0;
  if (digitalRead(KEY1) == LOW)
  {
    delay(100);
    if (digitalRead(KEY1) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY1) == LOW)
      {
        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY1_LONG_PRESS;
        }
      } //until open the key

      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY1_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}

char gokit_key2down(void)
{
  int unsigned long keep_time = 0;
  if (digitalRead(KEY2) == LOW)
  {
    delay(100);
    if (digitalRead(KEY2) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY2) == LOW) //until open the key
      {

        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY2_LONG_PRESS;
        }
      }

      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY2_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}

char gokit_keydown(void)
{
  char ret = 0;
  ret |= gokit_key2down();
  ret |= gokit_key1down();
  return ret;

}

/**
* KEY_Handle 
* @param none
* @return none
*/
void KEY_Handle(void)
{
  /*  Press for over than 3 second is Long Press  */
  switch (gokit_keydown())
  {
    case KEY1_SHORT_PRESS:
      mySerial.println(F("KEY1_SHORT_PRESS , Production Test Mode "));
      myGizwits.setBindMode(WIFI_PRODUCTION_TEST);
      break;
    case KEY1_LONG_PRESS:
      mySerial.println(F("KEY1_LONG_PRESS ,Wifi Reset"));
      myGizwits.setBindMode(WIFI_RESET_MODE);
      break;
    case KEY2_SHORT_PRESS:
      mySerial.println(F("KEY2_SHORT_PRESS Soft AP mode"));
      myGizwits.setBindMode(WIFI_SOFTAP_MODE);
      //Soft AP mode
      break;
    case KEY2_LONG_PRESS:
      mySerial.println(F("KEY2_LONG_PRESS ,AirLink mode"));
      myGizwits.setBindMode(WIFI_AIRLINK_MODE);
      //AirLink mode
      break;
    default:
      break;
  }
}

void wifiStatusHandle()
{
  if(myGizwits.wifiHasBeenSet(WIFI_SOFTAP))
  {
    mySerial.println(F("WIFI_SOFTAP!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_AIRLINK))
  {
    mySerial.println(F("WIFI_AIRLINK!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_STATION))
  {
    mySerial.println(F("WIFI_STATION!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_CON_ROUTER))
  {
    mySerial.println(F("WIFI_CON_ROUTER!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_DISCON_ROUTER))
  {
    mySerial.println(F("WIFI_DISCON_ROUTER!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_CON_M2M))
  {
    mySerial.println(F("WIFI_CON_M2M!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_DISCON_M2M))
  {
    mySerial.println(F("WIFI_DISCON_M2M!"));
  }
}





/**
* Serial Init , Gizwits Init  
* @param none
* @return none
*/
void setup() {
  // put your setup code here, to run once:

  mySerial.begin(9600);

  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  
  pixels.begin();
  pixels.show();

  myGizwits.begin();

  mySerial.println("GoKit init  OK \n");
  

}

/**
* Arduino loop 
* @param none
* @return none
*/

  
void loop() {  

//  setPixelColorAll(10,0,0);
//  delay(500);
//  setPixelColorAll(0,10,0);
//  delay(500);
//  setPixelColorAll(0,0,10);
//  delay(500);
  
  //Configure network
  //if(XXX) //Trigger Condition
  //myGizwits.setBindMode(0x02);  //0x01:Enter AP Mode;0x02:Enter Airlink Mode
  
  if(myGizwits.hasBeenSet(EVENT_LED_OnOff))
  {
    myGizwits.read(EVENT_LED_OnOff,&varR_LED_OnOff);//Address for storing data
    mySerial.println(F("EVENT_LED_OnOff"));
    mySerial.println(varR_LED_OnOff,DEC);
    if(varR_LED_OnOff==0)
      setPixelColorAll(0,0,0);
    else
      setPixelColorAll(varR_LED_R,varR_LED_G,varR_LED_B);

  }
  
  if(myGizwits.hasBeenSet(EVENT_LED_Color))
  {
    myGizwits.read(EVENT_LED_Color,&varR_LED_Color);//Address for storing data
    mySerial.println(F("EVENT_LED_Color"));
    mySerial.println(varR_LED_Color,DEC);
//    if(varR_LED_OnOff==1)
//    {
//      switch(varR_LED_Color)
//      {
//        case 1:
//        case 2:
//        case 3:
//        default:
//      }
//      
//    }

  }

  if(myGizwits.hasBeenSet(EVENT_LED_R))
  {
    myGizwits.read(EVENT_LED_R,&varR_LED_R);//Address for storing data
    mySerial.println(F("EVENT_LED_R"));
    mySerial.println(varR_LED_R,DEC);
    if(varR_LED_OnOff==1)
      setPixelColorAll(varR_LED_R,varR_LED_G,varR_LED_B);

  }

  if(myGizwits.hasBeenSet(EVENT_LED_G))
  {
    myGizwits.read(EVENT_LED_G,&varR_LED_G);//Address for storing data
    mySerial.println(F("EVENT_LED_G"));
    mySerial.println(varR_LED_G,DEC);
    if(varR_LED_OnOff==1)
      setPixelColorAll(varR_LED_R,varR_LED_G,varR_LED_B);

  }
 
  if(myGizwits.hasBeenSet(EVENT_LED_B))
  {
    myGizwits.read(EVENT_LED_B,&varR_LED_B);//Address for storing data
    mySerial.println(F("EVENT_LED_B"));
    mySerial.println(varR_LED_B,DEC);
    if(varR_LED_OnOff==1)
      setPixelColorAll(varR_LED_R,varR_LED_G,varR_LED_B);

  }


  //binary datapoint handle
  

  KEY_Handle();//key handle , network configure
  wifiStatusHandle();//WIFI Status Handle
  myGizwits.process();
}
