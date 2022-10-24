#include <Arduino.h>
#include <TinyGPS++.h>
#include <NDIRZ16.h>
#include <STM32RTC.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <stdio.h>

long t = 0;

SoftwareSerial Lora(D52, D53);
SoftwareSerial GPS(D48, D47);
SoftwareSerial O2(D32, A8);
SoftwareSerial CO2(D1, D0);


TinyGPSPlus g_gps;
TinyGPSCustom satsInView(g_gps, "GPGSV", 3);


//args lora:

String devEui = "6081F955064B7644";
String appEui = "6081F905C7B60F5C";
String appKey = "EC0F22DAEE0E9E93F717F31EC1F9D962";
String zone = "EU868";
String mode = "LWOTAA";
String dr = "DR3";
String ch3 = "3,867.1,DR0,DR5";
String ch4 = "4,867.3,DR0,DR5";
String ch5 = "5,867.5,DR0,DR5";
String ch6 = "6,867.7,DR0,DR5";
String ch7 = "7,867.9,DR0,DR5";

unsigned int timeout = 10000;
uint8_t resp;

String ATCommands[] = {
            "AT+DR=",
            "AT+ID=DevEui,",
            "AT+ID=AppEui,",
            "AT+ID=APPKEY,",
            "AT+MODE=",
            "AT+DR=",
            "AT+CH=",
            "AT+CH=",
            "AT+CH=",
            "AT+CH=",
            "AT+CH="
            
};

String loraParams[] = {zone, devEui, appEui, appKey, mode, dr, ch3, ch4, ch5, ch6, ch7};

String respBuff[] = {
            "+DR: EU868",
            "+ID:DevEui, 60:81:F9:55:06:4B:76:44",
            "+ID:AppEui, 60:81:F9:05:C7:B6:0F:5C",
            "+ID:APPKEY EC0F22DAEE0E9E93F717F31EC1F9D962",
            "+MODE","+DR: DR3",
            "+CH: 3,867100000,DR0:DR5",
            "+CH: 4,867300000,DR0:DR5",
            "+CH: 5,867500000,DR0:DR5",
            "+CH: 6,867700000,DR0:DR5",
            "+CH: 7,867900000,DR0:DR5",
            
};

// sending data

String msg = "AT+MSG=";
String expect = "+MSG=";
uint8_t val;
String arg = "";

struct packet{
  String lat;
  String lng;
  String year;
  String month;
  String day;
  String hr;
  String min;
  String sec;
  String o2;
  String co2;
};

packet params = {"", "", "", "", "", "", "", ""};

//O2 args

byte command[] = {0xFF, 0x01, 0x78, 0x04, 0x00, 0x00, 0x00, 0x00, 0x83};
byte config[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte O2resp[9] = {};
float o2concentration;

//CO2

NDIRZ16 CO2Sensor = NDIRZ16(&CO2);

void setup() {

  Serial.begin(9600);
  Lora.begin(9600);
  delay(100);

  LoraSetup(ATCommands, loraParams, respBuff, timeout);
  delay(100);

  GPS.begin(9600);
  delay(100);

  O2.begin(9600);
  O2.write(command, sizeof(command));
  O2.readBytes(O2resp, sizeof(O2resp));
  delay(100);

  CO2.begin(9600);
  delay(100);

}

void loop() {

  //GPS
  while (GPS.available() > 0)
  {
    byte gpsData = GPS.read();
    g_gps.encode(gpsData);
  }
  
  if (g_gps.altitude.isUpdated())
  if (g_gps.location.isUpdated())
  if (g_gps.date.isUpdated())
  if (g_gps.time.isUpdated())
 
 
  if (millis() > t + 1000)
  {
    arg = String(g_gps.location.lat(), 6);
    params.lat = arg;
    arg = "";

    arg = String(g_gps.location.lng(), 6);
    params.lng = arg;
    arg = "";

    arg = String(g_gps.date.year());
    params.year = arg;
    arg = "";

    arg = String(g_gps.date.month());
    params.month = arg;
    arg = "";

    arg = String(g_gps.date.day());
    params.day = arg;
    arg = "";

    arg = String(g_gps.time.hour());
    params.hr = arg;
    arg = "";

    arg = String(g_gps.time.minute());
    params.min = arg;
    arg = "";

    arg = String(g_gps.time.second());
    params.sec = arg;
    arg = "";

    delay(100);
  }

  //O2
  byte check = Sum(O2resp, sizeof(O2resp));
    if(check == O2resp[8]){
      o2concentration = (O2resp[2]*256 + O2resp[3])*0.1;
      arg.concat(o2concentration);
      params.o2 = arg;
      arg = "";
    }
    delay(100);


    if (CO2Sensor.measure()) 
    {
      params.co2 = CO2Sensor.ppm;
    }

  sendATcommand("AT+JOIN", "+JOIN", timeout);
  sendPacket(params, timeout);


  delay(50000); 

}


uint8_t sendATcommand(String ATcommand, String expected_answer, uint8_t timeout)
{
  uint8_t x=0,  answer=0;
    String response = "";
    unsigned long previous;
 
    delay(100); 
 
    while( Lora.available() > 0) Lora.read();    
 
    Lora.println(ATcommand);     
 
    previous = millis(); 
    
    do{        
        if(Serial.available() != 0){
            response.concat(Lora.read());
            x++;
            
            if (response.equals(expected_answer) != NULL){
                answer = 1;
            }
        }
    
    } while((answer == 0) && ((millis() - previous) < timeout));    
 
    return answer;
}

void LoraSetup(String ATCommands[], String loraParams[], String respBuff[], uint8_t timeout)
{ 
  uint8_t resp;
  for(int i = 0; i < 10; i++)
  {
    ATCommands[i] += loraParams[i];

    resp = sendATcommand(ATCommands[i], respBuff[i], timeout);
    Serial.println(resp);
  }
}

void sendPacket(packet p, uint8_t timeout)
{
  String msg = "AT+MSG=";
  String exp = "+MSG=";

  msg.concat(p.lat);
  msg += "-";
  msg.concat(p.lng);
  msg += "-";
  msg.concat(p.year);
  msg += "/";
  msg.concat(p.month);
  msg += "/";
  msg.concat(p.day);
  msg += "-";
  msg.concat(p.hr);
  msg += ":";
  msg.concat(p.min);
  msg += ":";
  msg.concat(p.sec);
  msg += "-";
  msg.concat(p.o2);
  msg += "-";
  msg.concat(p.co2);

  exp.concat(p.lat);
  exp += "-";
  exp.concat(p.lng);
  exp += "-";
  exp.concat(p.year);
  exp += "/";
  exp.concat(p.month);
  exp += "/";
  exp.concat(p.day);
  exp += "-";
  exp.concat(p.hr);
  exp += ":";
  exp.concat(p.min);
  exp += ":";
  exp.concat(p.sec);
  exp += "-";
  exp.concat(p.o2);
  exp += "-";
  exp.concat(p.co2);

  sendATcommand(msg, exp, timeout);

  delay(1000);

}

unsigned char Sum(unsigned char *i, unsigned char ln)
{
  unsigned char j, temp = 0;
  i +=1;
  for(j = 0; j < (ln -2); j++)
  {
    temp += *i;
    i++;
  }
  temp = (~temp) + 1;
  return (temp);
}
