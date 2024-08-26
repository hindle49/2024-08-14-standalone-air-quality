#include "standalone_air_quality.h"
using namespace std;

void setup() {
  // put your setup code here, to run once:
  setCpuFrequencyMhz(80);     //Minimum for wifi functions
  //setCpuFrequencyMhz(240);

#ifdef DEBUG
    // Open serial port if DEBUG set
  Serial.begin(115200);
  delay(1000);  //Wait 1 second to start the serial port
  Serial.flush();
#endif

  debug("\n\n\n");  // create a few blank lines
  debug("Air Quality Monitor = ");
  debug(VER);  // The software version
  debug("\n\n");

  pinMode(BUTTON_1, INPUT_PULLUP); // configure he four button pins as inputs
  //pinMode(BUTTON_2, INPUT_PULLUP);
  //pinMode(BUTTON_3, INPUT_PULLUP);
  //pinMode(BUTTON_4, INPUT_PULLUP);
  
  pinMode(RED_LED, OUTPUT);   // set LED pin to and output
  pinMode(GREEN_LED, OUTPUT); // set LED pin to and output

  digitalWrite(GREEN_LED, HIGH);  // flash the red led for 500ms
  delay(500);
  digitalWrite(GREEN_LED, LOW);
  delay(500);
  digitalWrite(GREEN_LED, HIGH); // flash the green led for 500ms
  delay(500);
  digitalWrite(GREEN_LED, LOW);
  delay(500);

  //OLED_StartUpMessages();    // Puts the Hindle message and version number of the display

  //startDS3231RTC();  // Start the External RTC module

  //LightSensor.begin();

  OTA_Request = false;                           // these might not be needed
  run_normal  = false;                           // these might not ve needed
  have_acquired_new_unix_time = false;          // false to indicate new unix time has not been acquired yet

  display_time_varable = 0;

  system_mode = STOP;    // auto will be the default

  wakeup_reason();  // Find out why this restarted. Not needed at present.

  // after these start-up modes, now start the last common vTasks

  // Button scanning
     // This is the task to scan buttons (turn off for now)
   /* xTaskCreatePinnedToCore(v_read_buttons,
      "BUTTONS",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1); //Core */


    // This is the task to keep refreshing the display
  xTaskCreatePinnedToCore(v_updateTheDisplay,
      "MAINDISPLAY",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1); //Core
    
  // This is the task to read the light level
  /*
  xTaskCreatePinnedToCore(vreadLightLevel,
      "LIGHTLEVEL",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1); //Core
  */




}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(15000 / portTICK_PERIOD_MS);  // release the task for 15 second
  // Have read that the main loop is set to a prority of 1, so run other stuff at lest P2.
}


void run_normal_start()
{
  send_data_now = true;

  system_mode = AUTO;
  system_mode_transition = true; //flag a new mode, in startup and needs an action.
  digitalWrite(GREEN_LED, HIGH);

  day_state = ERROR;
  day_state_transistion = true;  // This will invoke a change but the time will define what change.

  sent.pattern     = 0x00;
  sent.step        = 0xff;
  sent.sync        = 0x00;
  sent.brightness  = 0x00;
  sent.checksum    = 0x00;
  
  local.pattern    = 0x00;
  local.step       = 0xff;
  local.sync       = 0x00;
  local.brightness = 0x00;
  local.checksum   = 0x00;

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

    // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    debugln("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, lantern_1_Address, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer 

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    debugln("Failed to add peer");

  }

 vTaskDelay(2000 / portTICK_PERIOD_MS); // 2seconds to get the ESP-Now link working

   // This is the task to send data to the lanterns
  xTaskCreatePinnedToCore(v_send_data,
      "SENDATA",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1); //Core 

     // This is the main loop
  xTaskCreatePinnedToCore(vmain_loop,
      "VMAINLOOP",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1); //Core
}
