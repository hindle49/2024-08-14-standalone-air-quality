#include "standalone_air_quality.h"
using namespace std;

void setup() {
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

  pinMode(BUTTON_1, INPUT_PULLUP); // configure the four button pins as inputs
  
  //Keep the pull up during deep sleep 
  rtc_gpio_pullup_en(BUTTON_1);
  rtc_gpio_pulldown_dis(BUTTON_1); 
  
  //Now turn on the the hardware interrupt pin
  esp_sleep_enable_ext0_wakeup(BUTTON_1,0); //1 = High, 0 = Low
  
  
  
  pinMode(RED_LED, OUTPUT);   // set LED pin to and output
  pinMode(GREEN_LED, OUTPUT); // set LED pin to and output

  Wire.setClock(100000); // Force the I2C bus frequency

  wakeup_reason();  // Find out why this restarted. Not needed at present.

}

void loop() 
{
  vTaskDelay(60000 / portTICK_PERIOD_MS);  // release the task for 15 second
  // Have read that the main loop is set to a prority of 1, so run other stuff at lest P2.
}



