// A few ideas.
// After a poer up, check the button. Fork the software to enable the WiFi manager.
// On wake up the code will run as normal.

// V0001  Based on the lantern sw control
//        This version is to just getting started. This has been developed on my new Del laptop.
//        At this stage the OLED started during Setup and then goes on to the paramters from the 'sw control sketch'
//        This is going to take some effort to make the vTasks run witth a deep sleep
//
// V0002  Some simple first steps.
//        Reduce output pins to 1.
//        Reduce input pins to 1.
//        Flash external LED twice at power on
//        Show the power up messages on the OLED (Not in the correct part of the code)
//        Main loop checks for functions complete and time out.


const int VER = 2;
const char SKETCH_NAME[] = "Air Quality";

#define DEBUG true  // just set to enable debug, or not

#ifdef DEBUG
 #define debug(x) Serial.print(x)
 #define debugln(x) Serial.println(x)
#else
 #define debug(x)
 #define debugln(x)
#endif

//includes
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WiFi.h>
#include <esp_now.h>
#include <HTTPClient.h>  //#include <ESPHTTPClient.h>
//#include <JsonListener.h>
#include <time.h>
//#include <SSD1306Wire.h>  // This is for the newer smaller display
#include <SH1106Wire.h>   // This is for the older style larger display
#include <OLEDDisplayUi.h>
#include <Wire.h>           // for the i2c bus
#include <ArduinoOTA.h>
#include <DS3231.h>
#include <BH1750FVI.h>     // for the light sensor

#include <ESPmDNS.h>       // for OTA
#include <WiFiUdp.h>       // for OTA
#include <ArduinoOTA.h>    // for OTA

// wifi
const char* ssid = "BTHub5-KH6F";      // For OTA - Millfields
const char* password = "6be3d8bce6";   // For OTA - Millfields
//const char* ssid = "BT-KSA8P2";          // For OTA - Mum's
//const char* password = "h7kieNhPe7NCX";  // For OTA - Mum's

#define WiFi_Defs 5  // number of WiFi router definintions to try

#define WiFi_SSID2 "Ian's Hotspot"              // 2 router defininition - mobile hotspot
#define WiFi_PWD2 "11111111112222222222333333"  // 2 router password     - mobile hotspot
#define WiFi_SSID1 "BTHub5-KH6F"                // 1 router defininition - 3 Millfields
#define WiFi_PWD1 "6be3d8bce6"                  // 1 router password     - 3 Millfields
#define WiFi_SSID3 "Arduino"                    // 3 router defininition - Arduino
#define WiFi_PWD3 "11111111112222222222333333"  // 3 router password     - Arduino
#define WiFi_SSID4 "BT-KSA8P2"                  // 4 router defininition - mum's
#define WiFi_PWD4 "h7kixeNhPe7NCX"              // 4 router password     - mum's
#define WiFi_SSID5 "VM4487438"                  // 5 router defininition - Stockton office
#define WiFi_PWD5 "nmt3wwqqHgQv"                // 5 router password     - Stockton office

//Display
#define BRIGHTNESS_HIGH 255  // OLED brightness
#define BRIGHTNESS_LOW 100
#define CONTRAST_HIGH 255  // 0 to 255
#define CONTRAST_LOW 10
#define PRECHARGE_HIGH 0x33  // 0x30 to 0x33   was 255
#define PRECHARGE_LOW 0x30
#define COMDETECT_HIGH 255
#define COMDETECT_LOW 30

#define OLED_DISPLAY_WIDTH 128
#define OLED_DISPLAY_HEIGHT 64

#define GREEN_LED    GPIO_NUM_2   //LED_BUILTIN
#define RED_LED      GPIO_NUM_15
//#define EX_RTC_INT GPIO_NUM_33  // RTC_GPIO_08              ADC 1_5

#define BUTTON_1 GPIO_NUM_12
//#define BUTTON_2 GPIO_NUM_13
//#define BUTTON_3 GPIO_NUM_14
//#define BUTTON_4 GPIO_NUM_27

#define OFF   0
#define ON    1
#define AUTO  2
#define STOP  3  // Stop will be used for OTA and RTC modes. More for OLED working
   
                          // Have changed these so thay don't overlap with the previous definitions.
#define NIGHT         50  // This is the dead of night ~ 11pm until 6am
#define EARLY_MORNING 51  // This is 6am until 7am
#define MORNING       52  // This is 7am until 9am
#define DAY           53  // This is 9am until 2pm
#define EARLY_EVENING 54  // This is 2pm until 3pm
#define EVENING       55  // This is 3pm unilt 11pm
#define ERROR         99  // Error state

#define MAX_WAKE_TIME 15000 // 15 seconds on milli seconds. 
#define SLEEP_TIME    60    //time to deep sleep in seconds


// display / I2C bus
const int SDA_PIN = 21;    // I2C pins
const int SDC_PIN = 22;    // I2C pins
const int I2C_DISPLAY_ADDRESS = 0x3c;

const String WDAY_NAMES[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
const String MONTH_NAMES[] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);   // for the i2c light sensor

const char* NTP_SERVER = "pool.ntp.org";
const char* TZ_INFO    = "GMT+0BST-1,M3.5.0/01:00:00,M10.5.0/02:00:00";  // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;  //GMT+1:00

tm timeinfo;
time_t now;

// this is for the exRTC
DS3231 myRTC;
//RTClib myRTC;
bool century = false;
bool h12Flag;
bool pmFlag;

int          tm_hour = 0;
int          tm_min  = 0;
int          tm_sec  = 0;
float        tm_temperature = 0;
unsigned int tm_now = 0;


unsigned int present_seconds;

char strHour[3]        = "25";        //Two characters for the hour _0 to 23
char strMins[3]        = "66";        //Two characters for the minutes _0 to 59
char strTemperature[6] = "-99.9";     //Six characters for the temperature -99.9C to _99.9C

bool previous_connection_state;  // us this to record the last status of connection
bool WiFi_Connected;             // true if a WiFi connection has been made
byte WiFi_Pnt;                   // initialise WiFi router pointer
char *WIFI_PWD;                  // router password
char *WIFI_SSID;                 // router SSID
bool isOnce = true;              // use to run in the main loop just once
//bool  isMQTTmeaasge = true;     // Start up MQTT message will show once
unsigned long timeNow;           // store the millis time now.

#define DISPLAYUPDATERATE 300UL  // use this to control the display update rate 300ms

bool upDateDisplayNow;  // to flag the diaply needs updating.
bool send_data_now;     // to flag data needs sending

bool OTA_Request;  // This will be used to establish if a time expire deep sleep has occured
bool run_normal;   // Use this after the timed wake up to run the main code

bool have_acquired_new_unix_time;  // Use this to flag that a new unix time have been acquired while in OTA mode.

long averageLux;  // store light level

// Define the data structure
struct led_data {
  uint8_t pattern;
  uint8_t step;
  uint8_t sync;
  uint8_t brightness;
  uint8_t checksum;
  bool new_data_flag;
};

uint8_t checksum_calc;  // Use this to calulate the checksum value. Forces it to an unsigned int 8

// Define two instances for the led_data structure

led_data sent;  // This is for the data sent to the remote Lanterns
led_data local; // This is for the local data, which is being prepaired for transmission

//SSD1306Wire   display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
SH1106Wire      display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

bool buttonState[4] = {true, true, true, true}; // used to record the actual state of the button

bool button1JustChanged[4] = {false, false, false, false}; // used to show the button has just change state.

uint8_t lantern_1_Address[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// Peer info
esp_now_peer_info_t peerInfo;

uint8_t system_mode; // Use this to track the mode of operation. Off / Auto / On
bool    system_mode_transition; // Use this to flag a recent change.

uint8_t day_state;   // Use this is set what day mode the system is in
bool    day_state_transistion; // Use this to indicate a recent transistion.
uint8_t display_time_varable;  // use this to show the counter down time on the display



const uint8_t number_of_patter_steps = 37; // 0 to x
const uint8_t patterns[][5] = {
//                              
//  
                          
// Pattern, Step Rate,  Sync Level, Brightness,  Pattern Duration (in seconds decimal.)
  {0xf7,    0xff,       0x00,       0x10,        60},              // 1) Fixed white,  30 sec
  {0xf8,    0xff,       0x00,       0x10,        60},              // 2) Fixed red,    15 sec
  {0xf9,    0xff,       0x00,       0x10,        60},              // 3) Fixed blue,   30 sec
  {0xfa,    0xff,       0x00,       0x10,        60},              // 4) Fixed green,  45 sec
  {0xfb,    0xff,       0x00,       0x10,        60},              // 5) Fixed purple, 15 sec
  {0xfc,    0xff,       0x00,       0x10,        60},              // 6) Fixed yellow, 45 sec
  {0xfd,    0xff,       0x00,       0x10,        60},              // 7) Fixed cyan,   15 sec

  {0x0a,    0x40,       0x01,       0x00,        60},              // 8) white chase
  {0x0b,    0x40,       0x01,       0x00,        60},              // 9  red chase
  {0x0c,    0x40,       0x01,       0x00,        60},              // 10 blue chase
  {0x0d,    0x40,       0x01,       0x00,        60},              // 11 green chase
  {0x0e,    0x40,       0x01,       0x00,        60},              // 12 purpule chase
  {0x0f,    0x40,       0x01,       0x00,        60},              // 13 yellow chase
  {0x10,    0x40,       0x01,       0x00,        60},              // 14 cyan chase

  {0x11,    0xff,       0x00,       0x00,        60},              // 15 white spin}
  {0x12,    0xff,       0x00,       0x00,        60},              // 16 red spin
  {0x13,    0xff,       0x00,       0x00,        60},              // 17 blue spin
  {0x14,    0xff,       0x00,       0x00,        60},              // 18 green spin
  {0x15,    0xff,       0x00,       0x00,        60},              // 19 purple spin
  {0x16,    0xff,       0x00,       0x00,        60},              // 20 yellow spin
  {0x17,    0xff,       0x00,       0x00,        60},              // 21 cyan spin

  {0x60,    0x00,       0x05,       0x00,        60},              // 22 candle

  {0x80,    0x80,       0x00,       0x10,        60},              // 23 Lightening
  {0x81,    0x80,       0x00,       0x10,        60},              // 24 Lightening
  {0x82,    0x80,       0x00,       0x10,        60},              // 25 Lightening
  {0x83,    0x80,       0x00,       0x10,        60},              // 26 Lightening
  {0x84,    0x80,       0x00,       0x10,        60},              // 27 Lightening
  {0x85,    0x80,       0x00,       0x10,        60},              // 28 Lightening
  {0x86,    0x80,       0x00,       0x10,        60},              // 29 Lightening

  {0x88,    0x80,       0x00,       0x04,        60},              // 30 Lightening+
  {0x89,    0x80,       0x00,       0x04,        60},              // 31 Lightening+
  {0x8a,    0x80,       0x00,       0x04,        60},              // 32 Lightening+
  {0x8b,    0x80,       0x00,       0x04,        60},              // 23 Lightening+
  {0x8c,    0x80,       0x00,       0x04,        60},              // 34 Lightening+
  {0x8d,    0x80,       0x00,       0x04,        60},              // 35 Lightening+
  {0x8e,    0x80,       0x00,       0x04,        60},              // 36 Lightening+

  {0xa0,    0x80,       0x00,       0x18,        60},              // 37 Random

};

// New variables
bool air_quality_acquired = false; // Clear to show air quality not yet acquired
bool temp_hum_acquired    = false; // Clear to show temperature and humidty not yet acquired
bool display_updated      = false; // Clear to show the OLED has not been updated 


