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
// V0003  Move start up messages to the wake_reason.
//
// V0004  Test for deep sleep now in its own rtos task
//        main loop is doing nothing
//        new function to start all the VTasks
//        wake up messages starts, but then goes blank
//
// V0005  Sensor data being displayed via dub to the the serial port
// V0006  Add basic data to the screen
// V0007  Add a task to read the battery level.
//        The battery voltage is presented on anaologue pin A0, via two 1M resistors. Aka, A0 will be 0.5 * battery voltage
//        The unit is now showing a voltage as a figure on the display Vbatt 4.2. 
// V0008  An attempt to put the battery voltage as a bar graph at the bottom of the display.
//        This will need several #defines at the top of the OLED file.
//        Will also use the map() function to produce a scale
//        At the end:
//        1) Temperature as a bar graph     0 to  30
//        2) Humidity    as a bar graph    50 to 100
//        3) TVoc        as a bar graph     0 to 65000   ()
//        4) eCO2        as a bar graph   400 to 65000   (400 is good, > 1500 is bad, use 1500 as the upper bar graph limit
//        5) VBatt       as a bar graph     2.5 to 4.5
//
//        6) AQI         as a figure        1 to 5. Or use the words. Excellent, Good, Moderate, Poor, Unhealthy.
//        7) Wifi        Off / On / Error
//
//        At this stage will stop and save this verison 8 as there has been significant progress.
//        Initial bar graphs are work, but the screen is not optimised.
//        The air quality readings do not look great. Have placed a delay (5 seconds, via a #DEFINE) to delay the air quality reading.
//        This will have an effect on the battery life.
// V0009  Keep this short.
//        Just move the Temperature and Humidity readings around.
//        Put the Wifi message as a dummy.
//        TVOC and eCO2 values added
//        Constrants added back in.
// V0010  Add a simple level button press to turn on and off the Wifi function.
//        Stage 1
//        This will have to be a wake up in button press. There will be litle oppertinity to read the button during deep sleep. 
//        Stage 1 button working. Using the RTC feture to keep a pull up resistor active while in deep sleep.
//        This didn't work in an Input Only pin, so had to move it to a full IO pin. This now in GPIO_NUM_25.
//        The programming of the GPIO pin has also been done with the nomral GPIO references and not the RTC_GPIO references. 
//        Stage 2
//        Enable the button task and call the other tasks.
//        The button task will toggle the WiFi status.   NOTE: THE DEFINITIONS BELOW WILL HAVE TO STOP SETTING THE WIFI TO FALSE !!!!!!!!!!!!!!!!!
//        Becuase the Wifi status is stored in RTC memory, the value is lost during a power down.
// V0011  This will introduce the WiFi manager connection process.
//        This will need to come in two modes. Both modes will need to be working to get V11 functional.
//        Part 1
//        At power up, test the button ASAP, if pressed set a flag to say the WiFi details need to be updated.
//        As this is in the Wifi manager scripts, it will all have to be placed in a task and can be run from sperate parts of the sketch.
//        In a power on startup read the button. If pressed run a tangent set of functions.
//        Start the display, with the intial messages.
//        Then display the Wifi SETUP.
//        Start the WiFi task, remembering the flag will be set to reset of parmaters. As that is tested, clear the demand 'Request_New_WiFi' flag. 
//        Note: After the wifi has been checked, the system does not reset, so needs turning off and on again.
//        Part 2 
//        If the 'WIFI_enabled' flag is set, start the wifi connection task.
//        Note: When the power is turned off, the WiFi_Enabled flag will be lost. Therefore, on power up the WiFi will always be off.
// V0012  When the Wifi is connected, send all data to the MQTT broker.
//        As a reminder this will need another vTask and act on other flags, as well as setting a flag to indicate complete.
//        air_quality_acquired, temp_hum_acquired
//        data_sent_to_broker
// V0013  Just a few improvements.
//        Known issues. 
//        a) When acquiring a new Wifi configuration, the unit does not automatically reboot.                             Fixed.
//        b) Change the battery levels, and the battery bar graph. It needs to have got to zero for 3.6V.                 Change paramters for now.


//        c) When asking for a new Wifi, bypass the sign-on messages, to get more quickly to the WiFi message screen.     Not fixed.
//        d) Is the bettery scale too long?                                                                               Leave for now.
//        e) When Wifi is on, but not connecting, show a failure message.
//        f) The WiFi on/off mode drops occationally. No known reason.


const int VER = 13;
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
#include <math.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFi.h>

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include <esp_now.h>
#include "driver/rtc_io.h"
#include <HTTPClient.h>  //#include <ESPHTTPClient.h>
//#include <JsonListener.h>
#include <time.h>
//#include <SSD1306Wire.h>  // This is for the newer smaller display
#include <SH1106Wire.h>   // This is for the older style larger display
#include <OLEDDisplayUi.h>
//#include <Adafruit_GFX.h>  // Graphics library
#include <Wire.h>           // for the i2c bus
#include <ArduinoOTA.h>
#include <DS3231.h>
//#include <BH1750FVI.h>     // for the light sensor
#include <Adafruit_AHTX0.h>    // Temperature and Humidity
#include <ScioSense_ENS160.h>  // Air Quality - ENS160 library "Use the Adafruit fork version of the library"

#include <ESPmDNS.h>       // for OTA
#include <WiFiUdp.h>       // for OTA
#include <ArduinoOTA.h>    // for OTA



// wifi
//const char* ssid = "BTHub5-KH6F";      // For OTA - Millfields
//const char* password = "6be3d8bce6";   // For OTA - Millfields

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

//MQTT Broker
#define AIO_SERVER      "millfields.dyndns.org"   // This is still correct
#define AIO_SERVERPORT  1883                      // use 8883 for SSL - try it with 8883 
#define AIO_USERNAME    "homeassistant"
#define AIO_KEY         "Loo2me1aaR4Mee5Ohquae6ohChitoh4chogiehoanoongaeL1iMahr3gai4Oosux"   //put a new key in here


#define GREEN_LED    GPIO_NUM_2   //LED_BUILTIN
#define RED_LED      GPIO_NUM_15

#define BUTTON_1     GPIO_NUM_25  // This is the button for controlling the WiFi  35
//#define BUTTON_2   GPIO_NUM_13
//#define BUTTON_3   GPIO_NUM_14
//#define BUTTON_4   GPIO_NUM_27

#define VBATT_PIN A0 // Pin 4?, or 5. More lickly 5   was A0

#define BATTERY_END_VOLTAGE 3.6

#define MAX_WAKE_TIME   60000 // 15 seconds in milli seconds. 
#define SLEEP_TIME      60    //time to deep sleep in seconds

#define HOW_LONG_TO_WARM_UP 2000  // time that the system will wait before taking a reading in ms 2000 == 2 seconds


// display / I2C bus
const int SDA_PIN = 21;    // I2C pins
const int SDC_PIN = 22;    // I2C pins
const int I2C_DISPLAY_ADDRESS = 0x3c;

const String WDAY_NAMES[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
const String MONTH_NAMES[] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

//BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);   // for the i2c light sensor
//SSD1306Wire   display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
SH1106Wire      display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

Adafruit_AHTX0 aht;
ScioSense_ENS160      ens160(ENS160_I2CADDR_0);
//ScioSense_ENS160      ens160(ENS160_I2CADDR_1);


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
char WIFI_PWD[10];                  // router password
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


// New variables
bool air_quality_acquired = false; // Clear to show air quality not yet acquired
bool temp_hum_acquired    = false; // Clear to show temperature and humidty not yet acquired
bool display_updated      = false; // Clear to show the OLED has not been updated
bool wifi_button_timeout  = true;  // Assum for now that the button hasn't be pressed so deep sleep could be entred. The button taks will set the flaf to false.
bool request_new_WiFi     = false; // Used to check if Wifi manger should get a new SSID and PWD
bool wm_nonblocking       = false; // change to true to use non blocking
bool data_sent_to_brocker = false; // use this to flag if data has been sent to the MQTT broker

WiFiManager wm;                    // Set and instance of WiFIManager to wm
WiFiManagerParameter custom_field; // global param ( for non blocking w params )

RTC_DATA_ATTR bool WIFI_enabled; //= false; //Store this in RTC NVRAM. (Might have to remove the = false)

float         temperature = 0.0;
float         humidity    = 0.0;
unsigned int  Aqi         = 0;
unsigned int  Tvoc        = 0;
unsigned long Co2         = 0;
unsigned long Hp0         = 0;
unsigned long Hp1         = 0;
unsigned long Hp2         = 0;
unsigned long Hp3         = 0;

unsigned long battery_voltage = 0;

// Define C++ clases
// Create an ESP32 WiFiClient class.
WiFiClient /*Secure*/ client;   //Note: when this was set for WiFiClient Secure the MQTT would not connect
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish item_0   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/temperature");
Adafruit_MQTT_Publish item_1   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/humidity");
Adafruit_MQTT_Publish item_2   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/aqi");
Adafruit_MQTT_Publish item_3   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/tvoc");
Adafruit_MQTT_Publish item_4   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/co2");
Adafruit_MQTT_Publish item_5   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/hp0");
Adafruit_MQTT_Publish item_6   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/hp1");
Adafruit_MQTT_Publish item_7   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/hp2");
Adafruit_MQTT_Publish item_8   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/hp3");
Adafruit_MQTT_Publish item_9   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "stand_alone_air_quality/battery_voltage");




