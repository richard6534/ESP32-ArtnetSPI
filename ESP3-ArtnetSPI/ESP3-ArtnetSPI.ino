#include <SPI.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <ArtnetEther.h>
#include <FastLED.h>
#include "HTML_UrlEncode.h"
#include "ESPDMX.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
//---------------------------------------------------------------
// Set the static IP address to use if the DHCP fails to assign
// W5500 (5->CS,18->SCK,19->MISO,23->MOSI)
// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
EthernetClient client;
//---------------------------------------------------------------
ArtnetEtherReceiver artnet;
uint16_t universe1 = 0; 
uint16_t universe2 = 1;  
uint16_t universe3 = 2;  
uint16_t universe4 = 3;
uint16_t universe5 = 4;  
//---------------------------------------------------------------
// FastLED settings
const int numLeds = 512; // CHANGE FOR YOUR SETUP
const int numberOfChannels = numLeds * 3; // Total number of channels you want to receive (1 led = 3 channels)
//12, 14, 27, 26
const byte dataPin1 = 12;
const byte dataPin2 = 14;
const byte dataPin3 = 27;
const byte dataPin4 = 26;
CRGB leds1[numLeds];
CRGB leds2[numLeds];
CRGB leds3[numLeds];
CRGB leds4[numLeds];
//---------------------------------------------------------------
//LED Lan Universe1 Universe2 Universe3 Universe4->2,13,25,32,33,15
const byte Lan_LedPin = 2;
const byte UNI1_LedPin = 13;
const byte UNI2_LedPin = 25;
const byte UNI3_LedPin = 32;
const byte UNI4_LedPin = 33;
const byte UNI5_LedPin = 15;
//WebServer----------------------------------------------------------------
EthernetServer server(80);
String currentLine = "";                // make a String to hold incoming data from the client
String CMDString = "";
bool isCMD = false;
//DMX---------------------------------------------------------------
const byte DMXPin1 = 4;
DMXESPSerial dmx1;
//儲存程序----------------------------------------------------------
//EEPROM MAX 512
#define EEPROM_Status_SIZE 1
#define EEPROM_Status_Addr 0
byte IOT_Status = 0;
//StaticIP-----------------------------------------------------------
#define EEPROM_StaticIP_SIZE 20
#define EEPROM_StaticIP_Addr 1
//Subnet-------------------------------------------------------------------
#define EEPROM_Subnet_SIZE 20
#define EEPROM_Subnet_Addr 21
//Gateway-------------------------------------------------------------------
#define EEPROM_Gateway_SIZE 20
#define EEPROM_Gateway_Addr 41
//DMXPort_Universe-------------------------------------------------------------------
#define EEPROM_DMXPort_Universe_SIZE 1
#define EEPROM_DMXPort_Universe_Addr 61
//DMXPort_Model-------------------------------------------------------------------
#define EEPROM_DMXPort_Model_SIZE 1
#define EEPROM_DMXPort_Model_Addr 62
//DMXPort_Loop-------------------------------------------------------------------
#define EEPROM_DMXPort_Leds_SIZE 4
#define EEPROM_DMXPort_Leds_Addr 63
//SPI1_Universe-------------------------------------------------------------------
#define EEPROM_SPI1_Universe_SIZE 1
#define EEPROM_SPI1_Universe_Addr 67
//SPI1_Model-------------------------------------------------------------------
#define EEPROM_SPI1_Model_SIZE 1
#define EEPROM_SPI1_Model_Addr 68
//SPI1_Chip-------------------------------------------------------------------
#define EEPROM_SPI1_Chip_SIZE 1
#define EEPROM_SPI1_Chip_Addr 69
//SPI1_Leds-------------------------------------------------------------------
#define EEPROM_SPI1_Leds_SIZE 4
#define EEPROM_SPI1_Leds_Addr 70
//SPI2_Universe-------------------------------------------------------------------
#define EEPROM_SPI2_Universe_SIZE 1
#define EEPROM_SPI2_Universe_Addr 74
//SPI2_Model-------------------------------------------------------------------
#define EEPROM_SPI2_Model_SIZE 1
#define EEPROM_SPI2_Model_Addr 75
//SPI2_Chip-------------------------------------------------------------------
#define EEPROM_SPI2_Chip_SIZE 1
#define EEPROM_SPI2_Chip_Addr 76
//SPI2_Leds-------------------------------------------------------------------
#define EEPROM_SPI2_Leds_SIZE 4
#define EEPROM_SPI2_Leds_Addr 77
//SPI3_Universe-------------------------------------------------------------------
#define EEPROM_SPI3_Universe_SIZE 1
#define EEPROM_SPI3_Universe_Addr 81
//SPI3_Model-------------------------------------------------------------------
#define EEPROM_SPI3_Model_SIZE 1
#define EEPROM_SPI3_Model_Addr 82
//SPI3_Chip-------------------------------------------------------------------
#define EEPROM_SPI3_Chip_SIZE 1
#define EEPROM_SPI3_Chip_Addr 83
//SPI3_Leds-------------------------------------------------------------------
#define EEPROM_SPI3_Leds_SIZE 4
#define EEPROM_SPI3_Leds_Addr 84
//SPI4_Universe-------------------------------------------------------------------
#define EEPROM_SPI4_Universe_SIZE 1
#define EEPROM_SPI4_Universe_Addr 89
//SPI4_Model-------------------------------------------------------------------
#define EEPROM_SPI4_Model_SIZE 1
#define EEPROM_SPI4_Model_Addr 90
//SPI4_Chip-------------------------------------------------------------------
#define EEPROM_SPI4_Chip_SIZE 1
#define EEPROM_SPI4_Chip_Addr 91
//SPI4_Leds-------------------------------------------------------------------
#define EEPROM_SPI4_Leds_SIZE 4
#define EEPROM_SPI4_Leds_Addr 92
//-------------------------------------------------------------------
int EEPROM_Total = EEPROM_Status_SIZE + EEPROM_StaticIP_SIZE + EEPROM_Subnet_SIZE + EEPROM_Gateway_SIZE + 
                  EEPROM_DMXPort_Universe_SIZE + EEPROM_DMXPort_Model_SIZE + EEPROM_DMXPort_Leds_Addr + 
                  EEPROM_SPI1_Universe_SIZE + EEPROM_SPI1_Model_SIZE + EEPROM_SPI1_Chip_SIZE + EEPROM_SPI1_Leds_Addr + 
                  EEPROM_SPI2_Universe_SIZE + EEPROM_SPI2_Model_SIZE + EEPROM_SPI2_Chip_SIZE + EEPROM_SPI2_Leds_Addr + 
                  EEPROM_SPI3_Universe_SIZE + EEPROM_SPI3_Model_SIZE + EEPROM_SPI3_Chip_SIZE + EEPROM_SPI3_Leds_Addr +
                  EEPROM_SPI4_Universe_SIZE + EEPROM_SPI4_Model_SIZE + EEPROM_SPI4_Chip_SIZE + EEPROM_SPI4_Leds_Addr;
//-------------------------------------------------------------------
#pragma pack(push)
#pragma  pack(1)
//-----------------------------------------------------------------------
struct IOT_Config
{
  char  StaticIP[20];
  char  Subnet[20];
  char  Gateway[20];
  //0~5:Universe
  byte      DMXPort_Universe;
  //0:Single,1:Total
  byte      DMXPort_Model;
  int       DMXPort_Leds;
  //0~5:Universe
  byte      SPI1_Universe;
  //0:Single,1:Total
  byte      SPI1_Model;
  //0:WS2811,1:WS2812,2:WS2815
  byte      SPI1_Chip;
  int       SPI1_Leds;  
  //0~5:Universe
  byte      SPI2_Universe;
  //0:Single,1:Total
  byte      SPI2_Model;
  //0:WS2811,1:WS2812,2:WS2815
  byte      SPI2_Chip;
  int       SPI2_Leds;  
  //0~5:Universe
  byte      SPI3_Universe;
  //0:Single,1:Total
  byte      SPI3_Model;
  //0:WS2811,1:WS2812,2:WS2815
  byte      SPI3_Chip;
  int       SPI3_Leds;  
  //0~5:Universe
  byte      SPI4_Universe;
  //0:Single,1:Total
  byte      SPI4_Model;
  //0:WS2811,1:WS2812,2:WS2815
  byte      SPI4_Chip;
  int       SPI4_Leds;  
};
int IOT_Config_LEN = sizeof(IOT_Config);
//-----------------------------------------------------------------------
IOT_Config _Config;
//-----------------------------------------------------------------------
#pragma pack(pop)
//---------------------------------------------------------------
//LCD 
const byte Lcd_BtnPin = 34;
const byte Lcd_SDA = 21;
const byte Lcd_SCL = 22;
const byte lcdColumns = 16;
const byte lcdRows = 2;
byte lcdPage=0;
int BtnChange = LOW;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  LoadConfig();
  Ethernet_Initial();
  Arnet_Initial();
  FastLED_Initial();
  WebServer_Initial();
  LED_Initial();
  LCD_Initial();
  DMX_Initial();
}
//---------------------------------------------------------------
void loop() {
  Ethernet_Loop();
  Artnet_Loop();
  WebServer_Loop();    
  LCD_Loop();
  DMX_Loop();
  digitalWrite(UNI1_LedPin, LOW);
  digitalWrite(UNI2_LedPin, LOW);
  digitalWrite(UNI3_LedPin, LOW);
  digitalWrite(UNI4_LedPin, LOW);
  digitalWrite(UNI5_LedPin, LOW);
}
//---------------------------------------------------------------
void LoadConfig(){

    EEPROM.begin(EEPROM_Total);
    //判定是否設定過
    IOT_Status = EEPROM.read(EEPROM_Status_Addr);
    Serial.print("IOT_Status:");
    Serial.println(IOT_Status);    
    if(IOT_Status == 1)
    {//己設定過
      //staticIP
      for (int i = 0; i < EEPROM_StaticIP_SIZE; i++) {
        EEPROM.get(EEPROM_StaticIP_Addr + i, _Config.StaticIP[i]);
      }   
      for (int i = 0; i < EEPROM_Subnet_SIZE; i++) {
        EEPROM.get(EEPROM_Subnet_Addr + i, _Config.Subnet[i]);
      }
      for (int i = 0; i < EEPROM_Gateway_SIZE; i++) {
        EEPROM.get(EEPROM_Gateway_Addr + i, _Config.Gateway[i]);
      }     
      _Config.DMXPort_Universe = EEPROM.read(EEPROM_DMXPort_Universe_Addr);
      _Config.DMXPort_Model = EEPROM.read(EEPROM_DMXPort_Model_Addr);
      _Config.DMXPort_Leds = ReadIntFromEEPROM(EEPROM_DMXPort_Leds_Addr);

      _Config.SPI1_Universe = EEPROM.read(EEPROM_SPI1_Universe_Addr);
      _Config.SPI1_Model = EEPROM.read(EEPROM_SPI1_Model_Addr);
      _Config.SPI1_Chip = EEPROM.read(EEPROM_SPI1_Chip_Addr);
      _Config.SPI1_Leds = ReadIntFromEEPROM(EEPROM_SPI1_Leds_Addr);
            
      _Config.SPI2_Universe = EEPROM.read(EEPROM_SPI2_Universe_Addr);
      _Config.SPI2_Model = EEPROM.read(EEPROM_SPI2_Model_Addr);
      _Config.SPI2_Chip = EEPROM.read(EEPROM_SPI2_Chip_Addr);
      _Config.SPI2_Leds = ReadIntFromEEPROM(EEPROM_SPI2_Leds_Addr);
            
      _Config.SPI3_Universe = EEPROM.read(EEPROM_SPI3_Universe_Addr);
      _Config.SPI3_Model = EEPROM.read(EEPROM_SPI3_Model_Addr);
      _Config.SPI3_Chip = EEPROM.read(EEPROM_SPI3_Chip_Addr);
      _Config.SPI3_Leds = ReadIntFromEEPROM(EEPROM_SPI3_Leds_Addr);
          
      _Config.SPI4_Universe = EEPROM.read(EEPROM_SPI4_Universe_Addr);
      _Config.SPI4_Model = EEPROM.read(EEPROM_SPI4_Model_Addr);
      _Config.SPI4_Chip = EEPROM.read(EEPROM_SPI4_Chip_Addr);
      _Config.SPI4_Leds = ReadIntFromEEPROM(EEPROM_SPI4_Leds_Addr);                  
    }
    else
    {//未設定過
      strcpy(_Config.StaticIP,"192.168.0.10");//預設IP
      strcpy(_Config.Subnet,"255.255.255.0");//預設
      strcpy(_Config.Gateway,"192.168.0.1");//預設
      _Config.DMXPort_Universe=0;
      _Config.DMXPort_Model=0;
      _Config.DMXPort_Leds=0;      
      _Config.SPI1_Universe=0;
      _Config.SPI1_Model=0;
      _Config.SPI1_Chip=0;
      _Config.SPI1_Leds=0;
      _Config.SPI2_Universe=0;
      _Config.SPI2_Model=0;
      _Config.SPI2_Chip=0;
      _Config.SPI2_Leds=0;
      _Config.SPI3_Universe=0;
      _Config.SPI3_Model=0;
      _Config.SPI3_Chip=0;
      _Config.SPI3_Leds=0;
      _Config.SPI4_Universe=0;
      _Config.SPI4_Model=0;
      _Config.SPI4_Chip=0;
      _Config.SPI4_Leds=0;                  
    }
      //偵錯
      Serial.print("StaticIP:");
      Serial.println(_Config.StaticIP);
      Serial.print("Subnet:");
      Serial.println(_Config.Subnet);
      Serial.print("Gateway:");
      Serial.println(_Config.Gateway);

      Serial.print("DMXPort_Universe:");
      Serial.println(_Config.DMXPort_Universe);
      Serial.print("DMXPort_Model:");
      Serial.println(_Config.DMXPort_Model);
      Serial.print("DMXPort_Leds:");
      Serial.println(_Config.DMXPort_Leds);
      
      Serial.print("SPI1_Universe:");
      Serial.println(_Config.SPI1_Universe);
      Serial.print("SPI1_Model:");
      Serial.println(_Config.SPI1_Model);
      Serial.print("SPI1_Chip:");
      Serial.println(_Config.SPI1_Chip);
      Serial.print("SPI1_Leds:");
      Serial.println(_Config.SPI1_Leds);

      Serial.print("SPI2_Universe:");
      Serial.println(_Config.SPI2_Universe);
      Serial.print("SPI2_Model:");
      Serial.println(_Config.SPI2_Model);
      Serial.print("SPI2_Chip:");
      Serial.println(_Config.SPI2_Chip);
      Serial.print("SPI2_Leds:");
      Serial.println(_Config.SPI2_Leds);      

      Serial.print("SPI3_Universe:");
      Serial.println(_Config.SPI3_Universe);
      Serial.print("SPI3_Model:");
      Serial.println(_Config.SPI3_Model);
      Serial.print("SPI3_Chip:");
      Serial.println(_Config.SPI3_Chip);
      Serial.print("SPI3_Leds:");
      Serial.println(_Config.SPI3_Leds); 

      Serial.print("SPI4_Universe:");
      Serial.println(_Config.SPI4_Universe);
      Serial.print("SPI4_Model:");
      Serial.println(_Config.SPI4_Model);
      Serial.print("SPI4_Chip:");
      Serial.println(_Config.SPI4_Chip);
      Serial.print("SPI4_Leds:");
      Serial.println(_Config.SPI4_Leds); 
}
//---------------------------------------------------------------
void SaveConfig()
{
  //狀態
  EEPROM.write(EEPROM_Status_Addr, 1);
    //儲存StaticIP
  for (int i = 0; i < EEPROM_StaticIP_SIZE; i++) {
    EEPROM.put(EEPROM_StaticIP_Addr + i, _Config.StaticIP[i]);
  }
  for (int i = 0; i < EEPROM_Subnet_SIZE; i++) {
    EEPROM.put(EEPROM_Subnet_Addr + i, _Config.Subnet[i]);
  }
  for (int i = 0; i < EEPROM_Gateway_SIZE; i++) {
    EEPROM.put(EEPROM_Gateway_Addr + i, _Config.Gateway[i]);
  }  
  EEPROM.write(EEPROM_DMXPort_Universe_Addr, _Config.DMXPort_Universe);
  EEPROM.write(EEPROM_DMXPort_Model_Addr, _Config.DMXPort_Model);
  WriteIntIntoEEPROM(EEPROM_DMXPort_Leds_Addr, _Config.DMXPort_Leds);
    
  EEPROM.write(EEPROM_SPI1_Universe_Addr, _Config.SPI1_Universe);
  EEPROM.write(EEPROM_SPI1_Model_Addr, _Config.SPI1_Model);
  EEPROM.write(EEPROM_SPI1_Chip_Addr, _Config.SPI1_Chip);
  WriteIntIntoEEPROM(EEPROM_SPI1_Leds_Addr, _Config.SPI1_Leds);

  EEPROM.write(EEPROM_SPI2_Universe_Addr, _Config.SPI2_Universe);
  EEPROM.write(EEPROM_SPI2_Model_Addr, _Config.SPI2_Model);
  EEPROM.write(EEPROM_SPI2_Chip_Addr, _Config.SPI2_Chip);
  WriteIntIntoEEPROM(EEPROM_SPI2_Leds_Addr, _Config.SPI2_Leds);

  EEPROM.write(EEPROM_SPI3_Universe_Addr, _Config.SPI3_Universe);
  EEPROM.write(EEPROM_SPI3_Model_Addr, _Config.SPI3_Model);
  EEPROM.write(EEPROM_SPI3_Chip_Addr, _Config.SPI3_Chip);
  WriteIntIntoEEPROM(EEPROM_SPI3_Leds_Addr, _Config.SPI3_Leds);

  EEPROM.write(EEPROM_SPI4_Universe_Addr, _Config.SPI4_Universe);
  EEPROM.write(EEPROM_SPI4_Model_Addr, _Config.SPI4_Model);
  EEPROM.write(EEPROM_SPI4_Chip_Addr, _Config.SPI4_Chip);
  WriteIntIntoEEPROM(EEPROM_SPI4_Leds_Addr, _Config.SPI4_Leds);      

  EEPROM.commit();
}
//-------------------------------------------------------------------
void WriteIntIntoEEPROM(int address, int number)
{
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}
//-------------------------------------------------------------------
int ReadIntFromEEPROM(int address)
{
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}
//---------------------------------------------------------------
void LCD_Initial()
{
  pinMode(Lcd_BtnPin, INPUT);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  //lcd.backlight();  
  lcd.noBacklight();
}
//-------------------------------------------------------------------
void LCD_Loop()
{
  // read the state of the pushbutton value:
  int buttonState = digitalRead(Lcd_BtnPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    if(BtnChange==false){
      if(lcdPage==0){
          lcd.noBacklight();
          lcdPage++;          
      }
      else if(lcdPage==1)
      {
        lcd.backlight();
        lcd.clear();
        String Content="IP:" + String(_Config.StaticIP);
        LCD_SetText(0,0,Content);
        Content = "Mk:" + String(_Config.Subnet);
        LCD_SetText(0,1,Content);
        lcdPage++;
      }
      else if(lcdPage==2)
      {
        lcd.clear();
        String Content="Gw:" + String(_Config.Gateway);
        LCD_SetText(0,0,Content);
        lcdPage++;        
      }
      else if(lcdPage==3)
      {
        lcd.clear();
        String Content="DMX Uni:" + String(_Config.DMXPort_Universe);
        LCD_SetText(0,0,Content);
        Content="DMX Model:" + String(_Config.DMXPort_Model);
        LCD_SetText(0,1,Content);        
        lcdPage++;   
      }
      else if(lcdPage==4)
      {
        lcd.clear();
        String Content="DMX Loop:" + String(_Config.DMXPort_Leds);
        LCD_SetText(0,0,Content);
        lcdPage++; 
      }
      else if(lcdPage==5)
      {
        lcd.clear();
        String Content="SPI1 Uni:" + String(_Config.SPI1_Universe);
        LCD_SetText(0,0,Content);
        Content="SPI1 Model:" + String(_Config.SPI1_Model);
        LCD_SetText(0,1,Content);        
        lcdPage++; 
      }
      else if(lcdPage==6)
      {
        lcd.clear();
        String Content="SPI1 Chip:" + String(_Config.SPI1_Chip);
        LCD_SetText(0,0,Content);
        Content="SPI1 Leds:" + String(_Config.SPI1_Leds);
        LCD_SetText(0,1,Content);        
        lcdPage++;   
      }
      else if(lcdPage==7)
      {
        lcd.clear();
        String Content="SPI2 Uni:" + String(_Config.SPI2_Universe);
        LCD_SetText(0,0,Content);
        Content="SPI2 Model:" + String(_Config.SPI2_Model);
        LCD_SetText(0,1,Content);        
        lcdPage++; 
      }
      else if(lcdPage==8)
      {
        lcd.clear();
        String Content="SPI2 Chip:" + String(_Config.SPI2_Chip);
        LCD_SetText(0,0,Content);
        Content="SPI2 Leds:" + String(_Config.SPI2_Leds);
        LCD_SetText(0,1,Content);        
        lcdPage++;      
      }
      else if(lcdPage==9)
      {
        lcd.clear();
        String Content="SPI3 Uni:" + String(_Config.SPI3_Universe);
        LCD_SetText(0,0,Content);
        Content="SPI3 Model:" + String(_Config.SPI3_Model);
        LCD_SetText(0,1,Content);        
        lcdPage++; 
      }
      else if(lcdPage==10)
      {
        lcd.clear();
        String Content="SPI3 Chip:" + String(_Config.SPI3_Chip);
        LCD_SetText(0,0,Content);
        Content="SPI3 Leds:" + String(_Config.SPI3_Leds);
        LCD_SetText(0,1,Content);        
        lcdPage++;        
      }
      else if(lcdPage==11)
      {
        lcd.clear();
        String Content="SPI4 Uni:" + String(_Config.SPI4_Universe);
        LCD_SetText(0,0,Content);
        Content="SPI4 Model:" + String(_Config.SPI4_Model);
        LCD_SetText(0,1,Content);        
        lcdPage++; 
      }
      else if(lcdPage==12)
      {
        lcd.clear();
        String Content="SPI4 Chip:" + String(_Config.SPI4_Chip);
        LCD_SetText(0,0,Content);
        Content="SPI4 Leds:" + String(_Config.SPI4_Leds);
        LCD_SetText(0,1,Content);        
        lcdPage++;  
      }
      else if(lcdPage==13)
      {        
          lcd.noBacklight();
          lcdPage=0;                                         
      }      
      BtnChange = buttonState;
    }
  } 
  else BtnChange = LOW;
}
//---------------------------------------------------------------
void LCD_SetText(int x,int y,String Msg){
  lcd.setCursor(x, y);  
  lcd.print(Msg);
}
//---------------------------------------------------------------
void LED_Initial()
{
  pinMode(Lan_LedPin, OUTPUT);
  pinMode(UNI1_LedPin, OUTPUT);
  pinMode(UNI2_LedPin, OUTPUT);
  pinMode(UNI3_LedPin, OUTPUT);
  pinMode(UNI4_LedPin, OUTPUT);
  pinMode(UNI5_LedPin, OUTPUT);
  digitalWrite(Lan_LedPin, LOW);
  digitalWrite(UNI1_LedPin, LOW);
  digitalWrite(UNI2_LedPin, LOW);
  digitalWrite(UNI3_LedPin, LOW);    
  digitalWrite(UNI4_LedPin, LOW);  
  digitalWrite(UNI5_LedPin, LOW);  
}
//---------------------------------------------------------------
void FastLED_Initial()
{
  FastLED.addLeds<WS2815, dataPin1, RGB>(leds1, numLeds);
  FastLED.addLeds<WS2815, dataPin2, RGB>(leds2, numLeds);
  FastLED.addLeds<WS2815, dataPin3, RGB>(leds3, numLeds);
  FastLED.addLeds<WS2815, dataPin4, RGB>(leds4, numLeds);
  FastLED.setBrightness(255);
}
//---------------------------------------------------------------
void Arnet_Initial()
{
  artnet.begin();
  artnet.subscribeArtDmxUniverse(universe1, callback1);  
  artnet.subscribeArtDmxUniverse(universe2, callback2);
  artnet.subscribeArtDmxUniverse(universe3, callback3);
  artnet.subscribeArtDmxUniverse(universe4, callback4);
  artnet.subscribeArtDmxUniverse(universe5, callback5);
  Serial.println("ESP32 - Arnet Initialed");         
}
//---------------------------------------------------------------
void Artnet_Loop()
{
  artnet.parse();  
}
//---------------------------------------------------------------
void callback1(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
  Serial.print(data[0]);
  Serial.print(",");
  Serial.print(data[1]);
  Serial.print(",");
  Serial.print(data[2]);
  Serial.print(",");  
  Serial.print(data[3]);
  Serial.print(",");
  Serial.print(data[4]);
  Serial.print(",");
  Serial.print(data[5]);  
  Serial.print(",");
  Serial.print(size);    
  Serial.println();
  digitalWrite(UNI1_LedPin, HIGH);
  DMXPort_CallBack(0,data,size);
  SPI1_CallBack(0,data,size);
  SPI2_CallBack(0,data,size);
  SPI3_CallBack(0,data,size);
  SPI4_CallBack(0,data,size);
}
//---------------------------------------------------------------
void callback2(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
  digitalWrite(UNI2_LedPin, HIGH);  
  DMXPort_CallBack(1,data,size);
  SPI1_CallBack(1,data,size);
  SPI2_CallBack(1,data,size);
  SPI3_CallBack(1,data,size);
  SPI4_CallBack(1,data,size);
}
//---------------------------------------------------------------
void callback3(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
  digitalWrite(UNI3_LedPin, HIGH);
  DMXPort_CallBack(2,data,size);
  SPI1_CallBack(2,data,size);
  SPI2_CallBack(2,data,size);
  SPI3_CallBack(2,data,size);
  SPI4_CallBack(2,data,size);
}
//---------------------------------------------------------------
void callback4(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
  digitalWrite(UNI4_LedPin, HIGH);
  DMXPort_CallBack(3,data,size);
  SPI1_CallBack(3,data,size);
  SPI2_CallBack(3,data,size);
  SPI3_CallBack(3,data,size);
  SPI4_CallBack(3,data,size);
}
//---------------------------------------------------------------
void callback5(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
  digitalWrite(UNI5_LedPin, HIGH);
  DMXPort_CallBack(4,data,size);
  SPI1_CallBack(4,data,size);
  SPI2_CallBack(4,data,size);
  SPI3_CallBack(4,data,size);
  SPI4_CallBack(4,data,size);
}
//---------------------------------------------------------------
void DMXPort_CallBack(const byte Universe,const uint8_t *data, uint16_t size)
{
  if(_Config.DMXPort_Universe!=Universe)return;
  if(_Config.DMXPort_Model==0)
  {//Single
    for (int i = 0; i < 512; i++)
      dmx1.write(i+1,data[i]);
    dmx1.update();    
  }
  else
  {//Total
    int Count = 512/_Config.DMXPort_Leds;
    for(int i=0;i<Count;i++)
    {
      for(int j=0;j<_Config.DMXPort_Leds;j++)
      {
        dmx1.write(i*_Config.DMXPort_Leds+j+1,data[i*_Config.DMXPort_Leds+j]);
      }
    }
    dmx1.update();
  }
}
//---------------------------------------------------------------
void SPI1_CallBack(const byte Universe,const uint8_t *data, uint16_t size)
{
  if(_Config.SPI1_Universe!=Universe)return;
  if(_Config.SPI1_Model==0)
  {//Single
    int Count = size/3;
    for (int i = 0; i < Count; i++)
        leds1[i] = CRGB(data[i*3], data[i*3+1], data[i*3+2]);
    FastLED.show();   
  }
  else
  {//Total
    for (int i = 0; i < _Config.SPI1_Leds; i++)
        leds1[i] = CRGB(data[0], data[1], data[2]);
    FastLED.show();
  }
}
//---------------------------------------------------------------
void SPI2_CallBack(const byte Universe,const uint8_t *data, uint16_t size)
{
  if(_Config.SPI2_Universe!=Universe)return;
  if(_Config.SPI2_Model==0)
  {//Single
    int Count = size/3;
    for (int i = 0; i < Count; i++)
        leds2[i] = CRGB(data[i*3], data[i*3+1], data[i*3+2]);
    FastLED.show();    
  }
  else
  {//Total
    for (int i = 0; i < _Config.SPI2_Leds; i++)
        leds2[i] = CRGB(data[0], data[1], data[2]);
    FastLED.show();
  }
}
//---------------------------------------------------------------
void SPI3_CallBack(const byte Universe,const uint8_t *data, uint16_t size)
{
  if(_Config.SPI3_Universe!=Universe)return;
  if(_Config.SPI3_Model==0)
  {//Single
    int Count = size/3;
    for (int i = 0; i < Count; i++)
        leds3[i] = CRGB(data[i*3], data[i*3+1], data[i*3+2]);
    FastLED.show();    
  }
  else
  {//Total
    for (int i = 0; i < _Config.SPI3_Leds; i++)
        leds3[i] = CRGB(data[0], data[1], data[2]);
    FastLED.show();
  }
}
//---------------------------------------------------------------
void SPI4_CallBack(const byte Universe,const uint8_t *data, uint16_t size)
{
  if(_Config.SPI4_Universe!=Universe)return;
  if(_Config.SPI4_Model==0)
  {//Single
    int Count = size/3;
    for (int i = 0; i < Count; i++)
        leds4[i] = CRGB(data[i*3], data[i*3+1], data[i*3+2]);
    FastLED.show();    
  }
  else
  {//Total
    for (int i = 0; i < _Config.SPI4_Leds; i++)
        leds4[i] = CRGB(data[0], data[1], data[2]);
    FastLED.show();
  }
}
//---------------------------------------------------------------
void Ethernet_Initial()
{
  Ethernet.init(5);  // ESP32 with Adafruit FeatherWing Ethernet  
  IPAddress ip;
  ip.fromString(_Config.StaticIP);
  IPAddress dns;
  dns.fromString(_Config.Gateway);
  IPAddress gw;
  gw.fromString(_Config.Gateway);
  IPAddress sn;
  sn.fromString(_Config.Subnet);
  Ethernet.begin(mac,ip,dns,gw,sn);
  // check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
    Serial.println("Ethernet shield was not found");
  // check for Ethernet cable
  if (Ethernet.linkStatus() == LinkOFF){
    Serial.println("Ethernet cable is not connected.");
    digitalWrite(Lan_LedPin, LOW);
  }else{
    digitalWrite(Lan_LedPin, HIGH);
  }
  Serial.println("ESP32 - Ethernet Initialed");        
}
//-------------------------------------------------------------------
void Ethernet_Loop()
{
  if (Ethernet.linkStatus() == LinkOFF){
    digitalWrite(Lan_LedPin, LOW);
  }else{
    digitalWrite(Lan_LedPin, HIGH);
  }
}
//-------------------------------------------------------------------
void WebServer_Initial()
{
  server.begin();
}
//-------------------------------------------------------------------
void WebServer_Loop()
{
  EthernetClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      { // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n')
        { // if the byte is a newline character
          if (currentLine.length() == 0)
          {
            String Content = F("<!DOCTYPE html><html><head>");
            Content += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
            Content += F("<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />");
            Content += F("<style>");
            Content += F(" body { font-family: Arial, sans-serif; margin: 0; padding: 0px; background-color: #f4f4f4; color: #333; text-align: center; }");
            Content += F(" h1 { font-size: 20px; margin-bottom: 10px; }");
            Content += F(" form { background-color: #fff; padding: 10px; padding-right: 10px; border-radius: 0px; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); max-width: 400px; margin: auto; }");
            Content += F(" input[type='text'] { padding: 10px; margin: 0px 0; border: 1px solid #ccc; border-radius: 4px; font-size: 14px; }");
            Content += F(" select{width:100%; padding: 10px; margin: 0px 0; border: 1px solid #ccc; border-radius: 4px; font-size: 14px; }");
            Content += F(" input[type='submit'] { background-color: #4CAF50; color: white; border: none; padding: 12px; font-size: 16px; cursor: pointer; border-radius: 4px; width: 100%; }");
            Content += F(" input[type='submit']:hover { background-color: #45a049; }");
            Content += F(" .warning { margin-top: 20px; color: #d9534f; font-size: 14px; }");
            Content += F("</style></head>");
            Content += F("<body><h1>Configuration</h1>");                                                                        
            Content += F("<form method='get' action='http://");
            Content += _Config.StaticIP;
            Content += F("' width='100%'>"); 
            Content += F("<table width='100%'><tr>"); 
            Content += F("<td width='20%'><label >IP:</label></td>"); 
            Content += F("<td width='80%'><input type='text' id='IP' width='100%' name='IP' value='");
            Content += _Config.StaticIP;
            Content += F("'></td>");
            Content += F("</tr><tr>"); 
            Content += F("<td><label>Subnet:</label></td>"); 
            Content += F("<td><input type='text' id='Subnet' name='Subnet' value='");
            Content += _Config.Subnet;
            Content += F("'></td>");
            Content += F("</tr><tr>");
            Content += F("<td><label>Gateway:</label></td>");
            Content += F("<td width='80%'><input type='text' id='Gateway' name='Gateway' value='");
            Content += _Config.Gateway;
            Content += F("'></td>");
            Content += F("</tr></table>");
            client.print(Content);
            //DMX Port
            Content="";
            Content += F("<label >DMX Port</label>");
            Content += F("<table width='100%' style='background-color:#abd4ec;'><tr>");            
            Content += F("<td width='25%'><label>Universe</label><br>");
            Content += F("<select id='DMXPort_Universe' name='DMXPort_Universe'>");
            if(_Config.DMXPort_Universe==0)Content += F("<option value='0' SELECTED>1</option>");
            else Content += F("<option value='0'>1</option>");
            if(_Config.DMXPort_Universe==1)Content += F("<option value='1' SELECTED>2</option>");
            else Content += F("<option value='1'>2</option>");
            if(_Config.DMXPort_Universe==2)Content += F("<option value='2' SELECTED>3</option>");
            else Content += F("<option value='2'>3</option>");
            if(_Config.DMXPort_Universe==3)Content += F("<option value='3' SELECTED>4</option>");
            else Content += F("<option value='3'>4</option>");
            if(_Config.DMXPort_Universe==4)Content += F("<option value='4' SELECTED>5</option>");
            else Content += F("<option value='4'>5</option>");                                                
            Content += F("</select></td>");                                    
            Content += F("<td width='25%'><label>Model</label><br>");
            Content += F("<select id='DMXPort_Model' name='DMXPort_Model'>");
            if(_Config.DMXPort_Model==0)Content += F("<option value='0' SELECTED>Single</option>");
            else Content += F("<option value='0'>Single</option>");
            if(_Config.DMXPort_Model==1)Content += F("<option value='1' SELECTED>Total</option>");
            else Content += F("<option value='1'>Total</option>");            
            Content += F("</select></td>");
            Content += F("<td width='30%'><label>Loop of Channels</label><br>");
            Content += F("<input type='text' id='DMXPort_Leds' name='DMXPort_Leds' value='");
            Content += String(_Config.DMXPort_Leds);
            Content += F("'></td>");
            Content += F("</tr></table>");
            client.print(Content);
            //SPI 1
            Content="";
            Content += F("<label >SPI 1</label>");
            Content += F("<table width='100%' style='background-color:#abd4ec;'><tr>");            
            Content += F("<td width='50%'><label>Universe</label><br>");
            Content += F("<select id='SPI1_Universe' name='SPI1_Universe'>");
            if(_Config.SPI1_Universe==0)Content += F("<option value='0' SELECTED>1</option>");
            else Content += F("<option value='0'>1</option>");
            if(_Config.SPI1_Universe==1)Content += F("<option value='1' SELECTED>2</option>");
            else Content += F("<option value='1'>2</option>");
            if(_Config.SPI1_Universe==2)Content += F("<option value='2' SELECTED>3</option>");
            else Content += F("<option value='2'>3</option>");
            if(_Config.SPI1_Universe==3)Content += F("<option value='3' SELECTED>4</option>");
            else Content += F("<option value='3'>4</option>");
            if(_Config.SPI1_Universe==4)Content += F("<option value='4' SELECTED>5</option>");
            else Content += F("<option value='4'>5</option>");
            Content += F("</select></td>");                                    
            Content += F("<td width='50%'><label>Model</label><br>");
            Content += F("<select id='SPI1_Model' name='SPI1_Model'>");
            if(_Config.SPI1_Model==0)Content += F("<option value='0' SELECTED>Single</option>");
            else Content += F("<option value='0'>Single</option>");
            if(_Config.SPI1_Model==1)Content += F("<option value='1' SELECTED>Total</option>");
            else Content += F("<option value='1'>Total</option>"); 
            Content += F("</select></td>");
            Content += F("</tr><tr>");            
            Content += F("<td width='50%'><label>Chip</label><br>");
            Content += F("<select id='SPI1_Chip' name='SPI1_Chip'>");
            if(_Config.SPI1_Chip==0)Content += F("<option value='0' SELECTED>WS2811</option>");
            else Content += F("<option value='0'>WS2811</option>");
            if(_Config.SPI1_Chip==1)Content += F("<option value='1' SELECTED>WS2812</option>");
            else Content += F("<option value='1'>WS2812</option>");
            if(_Config.SPI1_Chip==2)Content += F("<option value='2' SELECTED>WS2815</option>");
            else Content += F("<option value='2'>WS2815</option>");
            Content += F("</select></td>");
            Content += F("<td width='50%'><label>Number of LEDs(Max.680)</label><br>");
            Content += F("<input type='text' id='SPI1_Leds' name='SPI1_Leds' value='");
            Content += String(_Config.SPI1_Leds);
            Content += F("'></td>");
            Content += F("</tr></table>");
            client.print(Content);
            //SPI 2
            Content="";
            Content += F("<label >SPI 2</label>");
            Content += F("<table width='100%' style='background-color:#abd4ec;'><tr>");            
            Content += F("<td width='50%'><label>Universe</label><br>");
            Content += F("<select id='SPI2_Universe' name='SPI2_Universe'>");
            if(_Config.SPI2_Universe==0)Content += F("<option value='0' SELECTED>1</option>");
            else Content += F("<option value='0'>1</option>");
            if(_Config.SPI2_Universe==1)Content += F("<option value='1' SELECTED>2</option>");
            else Content += F("<option value='1'>2</option>");
            if(_Config.SPI2_Universe==2)Content += F("<option value='2' SELECTED>3</option>");
            else Content += F("<option value='2'>3</option>");
            if(_Config.SPI2_Universe==3)Content += F("<option value='3' SELECTED>4</option>");
            else Content += F("<option value='3'>4</option>");
            if(_Config.SPI2_Universe==4)Content += F("<option value='4' SELECTED>5</option>");
            else Content += F("<option value='4'>5</option>");
            Content += F("</select></td>");                                    
            Content += F("<td width='50%'><label>Model</label><br>");
            Content += F("<select id='SPI2_Model' name='SPI2_Model'>");
            if(_Config.SPI2_Model==0)Content += F("<option value='0' SELECTED>Single</option>");
            else Content += F("<option value='0'>Single</option>");
            if(_Config.SPI2_Model==1)Content += F("<option value='1' SELECTED>Total</option>");
            else Content += F("<option value='1'>Total</option>"); 
            Content += F("</select></td>");
            Content += F("</tr><tr>");            
            Content += F("<td width='50%'><label>Chip</label><br>");
            Content += F("<select id='SPI2_Chip' name='SPI2_Chip'>");
            if(_Config.SPI2_Chip==0)Content += F("<option value='0' SELECTED>WS2811</option>");
            else Content += F("<option value='0'>WS2811</option>");
            if(_Config.SPI2_Chip==1)Content += F("<option value='1' SELECTED>WS2812</option>");
            else Content += F("<option value='1'>WS2812</option>");
            if(_Config.SPI2_Chip==2)Content += F("<option value='2' SELECTED>WS2815</option>");
            else Content += F("<option value='2'>WS2815</option>");
            Content += F("</select></td>");
            Content += F("<td width='50%'><label width='5%'>Number of LEDs(Max.680)</label><br>");
            Content += F("<input type='text' id='SPI2_Leds' name='SPI2_Leds' value='");
            Content += String(_Config.SPI2_Leds);
            Content += F("'></td>");
            Content += F("</tr></table>");
            client.print(Content);
            //SPI 3
            Content="";            
            Content += F("<label >SPI 3</label>");
            Content += F("<table width='100%' style='background-color:#abd4ec;'><tr>");            
            Content += F("<td width='50%'><label>Universe</label><br>");
            Content += F("<select id='SPI3_Universe' name='SPI3_Universe'>");
            if(_Config.SPI3_Universe==0)Content += F("<option value='0' SELECTED>1</option>");
            else Content += F("<option value='0'>1</option>");
            if(_Config.SPI3_Universe==1)Content += F("<option value='1' SELECTED>2</option>");
            else Content += F("<option value='1'>2</option>");
            if(_Config.SPI3_Universe==2)Content += F("<option value='2' SELECTED>3</option>");
            else Content += F("<option value='2'>3</option>");
            if(_Config.SPI3_Universe==3)Content += F("<option value='3' SELECTED>4</option>");
            else Content += F("<option value='3'>4</option>");
            if(_Config.SPI3_Universe==4)Content += F("<option value='4' SELECTED>5</option>");
            else Content += F("<option value='4'>5</option>");
            Content += F("</select></td>");                                    
            Content += F("<td width='50%'><label>Model</label><br>");
            Content += F("<select id='SPI3_Model' name='SPI3_Model'>");
            if(_Config.SPI3_Model==0)Content += F("<option value='0' SELECTED>Single</option>");
            else Content += F("<option value='0'>Single</option>");
            if(_Config.SPI3_Model==1)Content += F("<option value='1' SELECTED>Total</option>");
            else Content += F("<option value='1'>Total</option>");
            Content += F("</select></td>");
            Content += F("</tr><tr>");            
            Content += F("<td width='50%'><label>Chip</label><br>");
            Content += F("<select id='SPI3_Chip' name='SPI3_Chip'>");
            if(_Config.SPI3_Chip==0)Content += F("<option value='0' SELECTED>WS2811</option>");
            else Content += F("<option value='0'>WS2811</option>");
            if(_Config.SPI3_Chip==1)Content += F("<option value='1' SELECTED>WS2812</option>");
            else Content += F("<option value='1'>WS2812</option>");
            if(_Config.SPI3_Chip==2)Content += F("<option value='2' SELECTED>WS2815</option>");
            else Content += F("<option value='2'>WS2815</option>");
            Content += F("</select></td>");
            Content += F("<td width='50%'><label width='5%'>Number of LEDs(Max.680)</label><br>");
            Content += F("<input type='text' id='SPI3_Leds' name='SPI3_Leds' value='");
            Content += String(_Config.SPI3_Leds); 
            Content += F("'></td>");
            Content += F("</tr></table>");
            client.print(Content);
            //SPI 4
            Content="";
            Content += F("<label >SPI 4</label>");
            Content += F("<table width='100%' style='background-color:#abd4ec;'><tr>");            
            Content += F("<td width='50%'><label>Universe</label><br>");
            Content += F("<select id='SPI4_Universe' name='SPI4_Universe'>");
            if(_Config.SPI4_Universe==0)Content += F("<option value='0' SELECTED>1</option>");
            else Content += F("<option value='0'>1</option>");
            if(_Config.SPI4_Universe==1)Content += F("<option value='1' SELECTED>2</option>");
            else Content += F("<option value='1'>2</option>");
            if(_Config.SPI4_Universe==2)Content += F("<option value='2' SELECTED>3</option>");
            else Content += F("<option value='2'>3</option>");
            if(_Config.SPI4_Universe==3)Content += F("<option value='3' SELECTED>4</option>");
            else Content += F("<option value='3'>4</option>");
            if(_Config.SPI4_Universe==4)Content += F("<option value='4' SELECTED>5</option>");
            else Content += F("<option value='4'>5</option>");
            Content += F("</select></td>");                                    
            Content += F("<td width='50%'><label>Model</label><br>");
            Content += F("<select id='SPI4_Model' name='SPI4_Modle'>");
            if(_Config.SPI4_Model==0)Content += F("<option value='0' SELECTED>Single</option>");
            else Content += F("<option value='0'>Single</option>");
            if(_Config.SPI4_Model==1)Content += F("<option value='1' SELECTED>Total</option>");
            else Content += F("<option value='1'>Total</option>");
            Content += F("</select></td>");
            Content += F("</tr><tr>");            
            Content += F("<td width='50%'><label>Chip</label><br>");
            Content += F("<select id='SPI4_Chip' name='SPI4_Chip'>");
            if(_Config.SPI4_Chip==0)Content += F("<option value='0' SELECTED>WS2811</option>");
            else Content += F("<option value='0'>WS2811</option>");
            if(_Config.SPI4_Chip==1)Content += F("<option value='1' SELECTED>WS2812</option>");
            else Content += F("<option value='1'>WS2812</option>");
            if(_Config.SPI4_Chip==2)Content += F("<option value='2' SELECTED>WS2815</option>");
            else Content += F("<option value='2'>WS2815</option>");
            Content += F("</select></td>");
            Content += F("<td width='50%'><label width='5%'>Number of LEDs(Max.680)</label><br>");
            Content += F("<input type='text' id='SPI4_Leds' name='SPI4_Leds' value='");
            Content += String(_Config.SPI4_Leds); 
            Content += F("'></td>");
            Content += F("</tr></table>");
            client.print(Content);
                        
            Content="";            
            Content += F("<br><input type='submit' value='Save'>");
            Content += F("</form>");
            Content += F("<div class='warning'>");
            Content += F("<p><strong>Warning:</strong>儲存後系統將重新啟動</p>");
            Content += F("</div></body></html>");
            //Serial.print(Content);
            client.print(Content);
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        { // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
          if (isCMD == true) {
            CMDString += c;
          }
        }
        if (currentLine.endsWith("GET /?"))
        {
          CMDString = "";
          isCMD = true;
        }
        if (currentLine.endsWith("HTTP/1.1"))
        {
          isCMD = false;
        }
      }
    }
    //解釋網頁指令
    if (CMDString.length() != 0)
    {
      CMDString.replace("HTTP/1.1", "");
      int Count = str_split_len(CMDString, '&');
      String *irString = new String[Count];
      
      int found = 0;
      int strIndex[] = {0, -1};
      int maxIndex = CMDString.length();
      for (int i = 0; i < maxIndex; i++)
      {
        if (CMDString.charAt(i) == '&' || i == (maxIndex - 1))
        {
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i + 1 : i;
          if (i == (maxIndex - 1))
          {
            irString[found] = CMDString.substring(strIndex[0], strIndex[1] + 1);
            //Serial.println(irString[found]);
          }
          else
          {
            irString[found] = CMDString.substring(strIndex[0], strIndex[1]);
            //Serial.println(irString[found]);
          }
          found++;
        }
      }
      /*
      IP=192.168.0.10
      Subnet=255.255.255.0
      Gateway=192.168.0.1
      DMXPort_Universe=0
      DMXPort_Model=0
      DMXPort_Leds=0
      SPI1_Universe=1
      SPI1_Model=0
      SPI1_Chip=1
      SPI1_Leds=0
      SPI2_Universe=2
      SPI2_Model=0
      SPI2_Chip=1
      SPI2_Leds=0
      SPI3_Universe=3
      SPI3_Model=0
      SPI3_Chip=2
      SPI3_Leds=0
      SPI4_Universe=4
      SPI4_Modle=0
      SPI4_Chip=2
      SPI4_Leds=0
      */
      //Config
      String StaticIP = irString[0];
      StaticIP.replace("IP=", "");
      strcpy(_Config.StaticIP, StaticIP.c_str());
      Serial.println(irString[0]);
      Serial.println(_Config.StaticIP);
      
      String Subnet = irString[1];
      Subnet.replace("Subnet=", "");
      strcpy(_Config.Subnet, Subnet.c_str());
      Serial.println(irString[1]);
      Serial.println(_Config.Subnet);      

      String Gateway = irString[2];
      Gateway.replace("Gateway=", "");
      strcpy(_Config.Gateway, Gateway.c_str());
      Serial.println(irString[2]);
      Serial.println(_Config.Gateway);          
      //DMX Port
      String DMXPort_Universe = irString[3];
      DMXPort_Universe.replace("DMXPort_Universe=", "");
      _Config.DMXPort_Universe = DMXPort_Universe.toInt();
      Serial.println(irString[3]);
      Serial.println(_Config.DMXPort_Universe);   
      
      String DMXPort_Model = irString[4];
      DMXPort_Model.replace("DMXPort_Model=", "");
      _Config.DMXPort_Model = DMXPort_Model.toInt();  
      Serial.println(irString[4]);
      Serial.println(_Config.DMXPort_Model);  
      
      String DMXPort_Leds = irString[5];
      DMXPort_Leds.replace("DMXPort_Leds=", "");
      _Config.DMXPort_Leds = DMXPort_Leds.toInt();
      Serial.println(irString[5]);
      Serial.println(_Config.DMXPort_Leds);  
      //SPI 1
      String SPI1_Universe = irString[6];
      SPI1_Universe.replace("SPI1_Universe=", "");
      _Config.SPI1_Universe = SPI1_Universe.toInt();
      Serial.println(irString[6]);
      Serial.println(_Config.SPI1_Universe); 
            
      String SPI1_Model = irString[7];
      SPI1_Model.replace("SPI1_Model=", "");
      _Config.SPI1_Model = SPI1_Model.toInt();  
      Serial.println(irString[7]);
      Serial.println(_Config.SPI1_Model); 
      
      String SPI1_Chip = irString[8];
      SPI1_Chip.replace("SPI1_Chip=", "");
      _Config.SPI1_Chip = SPI1_Chip.toInt();
      Serial.println(irString[8]);
      Serial.println(_Config.SPI1_Chip); 
      
      String SPI1_Leds = irString[9];
      SPI1_Leds.replace("SPI1_Leds=", "");
      _Config.SPI1_Leds = SPI1_Leds.toInt(); 
      Serial.println(irString[9]);
      Serial.println(_Config.SPI1_Leds);        
      //SPI 2
      String SPI2_Universe = irString[10];
      SPI2_Universe.replace("SPI2_Universe=", "");
      _Config.SPI2_Universe = SPI2_Universe.toInt();
      Serial.println(irString[10]);
      Serial.println(_Config.SPI2_Universe);        
      
      String SPI2_Model = irString[11];
      SPI2_Model.replace("SPI2_Model=", "");
      _Config.SPI2_Model = SPI2_Model.toInt(); 
      Serial.println(irString[11]);
      Serial.println(_Config.SPI2_Model);         
      
      String SPI2_Chip = irString[12];
      SPI2_Chip.replace("SPI2_Chip=", "");
      _Config.SPI2_Chip = SPI2_Chip.toInt();
      Serial.println(irString[12]);
      Serial.println(_Config.SPI2_Chip);          
      
      String SPI2_Leds = irString[13];
      SPI2_Leds.replace("SPI2_Leds=", "");
      _Config.SPI2_Leds = SPI2_Leds.toInt(); 
      Serial.println(irString[13]);
      Serial.println(_Config.SPI2_Leds);   
             
      //SPI 3
      String SPI3_Universe = irString[14];
      SPI3_Universe.replace("SPI3_Universe=", "");
      _Config.SPI3_Universe = SPI3_Universe.toInt();
      Serial.println(irString[14]);
      Serial.println(_Config.SPI3_Universe); 
      
      String SPI3_Model = irString[15];
      SPI3_Model.replace("SPI3_Model=", "");
      _Config.SPI3_Model = SPI3_Model.toInt(); 
      Serial.println(irString[15]);
      Serial.println(_Config.SPI3_Model);        
      
      String SPI3_Chip = irString[16];
      SPI3_Chip.replace("SPI3_Chip=", "");
      _Config.SPI3_Chip = SPI3_Chip.toInt();
      Serial.println(irString[16]);
      Serial.println(_Config.SPI3_Chip);         
      
      String SPI3_Leds = irString[17];
      SPI3_Leds.replace("SPI3_Leds=", "");
      _Config.SPI3_Leds = SPI3_Leds.toInt();
      Serial.println(irString[17]);
      Serial.println(_Config.SPI3_Leds);    
            
       //SPI 4
      String SPI4_Universe = irString[18];
      SPI4_Universe.replace("SPI4_Universe=", "");
      _Config.SPI4_Universe = SPI4_Universe.toInt();
      Serial.println(irString[18]);
      Serial.println(_Config.SPI4_Universe);      
      
      String SPI4_Model = irString[19];
      SPI4_Model.replace("SPI4_Model=", "");
      _Config.SPI4_Model = SPI4_Model.toInt();  
      Serial.println(irString[19]);
      Serial.println(_Config.SPI4_Model);
      
      String SPI4_Chip = irString[20];
      SPI4_Chip.replace("SPI4_Chip=", "");
      _Config.SPI4_Chip = SPI4_Chip.toInt();
      Serial.println(irString[20]);
      Serial.println(_Config.SPI4_Chip);
      
      String SPI4_Leds = irString[21];
      SPI4_Leds.replace("SPI4_Leds=", "");
      _Config.SPI4_Leds = SPI4_Leds.toInt();
      Serial.println(irString[21]);
      Serial.println(_Config.SPI4_Leds);      
      //偵錯
      /*
      Serial.print("StaticIP:");
      Serial.println(_Config.StaticIP);
      Serial.print("Subnet:");
      Serial.println(_Config.Subnet);
      Serial.print("Gateway:");
      Serial.println(_Config.Gateway);
      
      Serial.print("DMXPort_Universe:");
      Serial.println(_Config.DMXPort_Universe);
      Serial.print("DMXPort_Modle:");
      Serial.println(_Config.DMXPort_Model);
      Serial.print("DMXPort_Leds:");
      Serial.println(_Config.DMXPort_Leds);

      Serial.print("SPI1_Universe:");
      Serial.println(_Config.SPI1_Universe);
      Serial.print("SPI1_Modle:");
      Serial.println(_Config.SPI1_Model);
      Serial.print("SPI1_Chip:");
      Serial.println(_Config.SPI1_Chip);
      Serial.print("SPI1_Leds:");
      Serial.println(_Config.SPI1_Leds);      

      Serial.print("SPI2_Universe:");
      Serial.println(_Config.SPI2_Universe);
      Serial.print("Uni3_Model:");
      Serial.println(_Config.SPI2_Model);
      Serial.print("Uni3_Chip:");
      Serial.println(_Config.SPI2_Chip);
      Serial.print("Uni3_Leds:");
      Serial.println(_Config.SPI2_Leds); 

      Serial.print("SPI3_Universe:");
      Serial.println(_Config.SPI3_Universe);
      Serial.print("SPI3_Modle:");
      Serial.println(_Config.SPI3_Model);
      Serial.print("SPI3_Chip:");
      Serial.println(_Config.SPI3_Chip);
      Serial.print("SPI3_Leds:");
      Serial.println(_Config.SPI3_Leds);  

      Serial.print("SPI4_Universe:");
      Serial.println(_Config.SPI4_Universe);
      Serial.print("SPI4_Modle:");
      Serial.println(_Config.SPI4_Model);
      Serial.print("SPI4_Chip:");
      Serial.println(_Config.SPI4_Chip);
      Serial.print("SPI4_Leds:");
      Serial.println(_Config.SPI4_Leds);     
      */                      
      //儲存設定及狀態
      SaveConfig();
      //回復網頁
      client.print("<!DOCTYPE html><html>");
      client.print("<head>");
      client.print("<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />");
      client.print("<meta http-equiv='refresh' content='3;url=http://");
      client.print(_Config.StaticIP);
      client.print("' />");
      client.print("<title>設備註冊頁面</title>");
      client.print("</head>");
      client.print("<body>");
      client.print("  設定成功 ");
      client.print("</body>");
      client.print("</form>");
      client.print("</html>");
      // The HTTP response ends with another blank line:
      client.println();
      //關閉連線
      client.stop();
      delay(3000);
      //重開機
      ESP.restart();
    }
  }
}
//-------------------------------------------------------------------------
int str_split_len(String _data, char sp) {
  // 計算字串內總共有幾個數
  int len = 1;
  for (int i = 0; i < _data.length(); ++i) {
    if ( _data.charAt(i) == sp)
      len++;
  }
  return len;
}
//---------------------------------------------------------------------
void DMX_Initial() 
{
  dmx1.init(512,DMXPin1);           //initialization for complete bus
  Serial.println("SerialToDmx ready");
}
//---------------------------------------------------------------------
void DMX_Loop() {

}
//---------------------------------------------------------------
