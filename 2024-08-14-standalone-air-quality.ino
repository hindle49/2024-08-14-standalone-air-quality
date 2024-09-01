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

  pinMode(BUTTON_1, INPUT_PULLUP); // configure he four button pins as inputs
  
  pinMode(RED_LED, OUTPUT);   // set LED pin to and output
  pinMode(GREEN_LED, OUTPUT); // set LED pin to and output

  

  wakeup_reason();  // Find out why this restarted. Not needed at present.

}

void loop() 
{
  vTaskDelay(60000 / portTICK_PERIOD_MS);  // release the task for 15 second
  // Have read that the main loop is set to a prority of 1, so run other stuff at lest P2.
}



