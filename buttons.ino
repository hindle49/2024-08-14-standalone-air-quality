// This will be a simple task that will run a counter for a few two seconds to prevent debounce.
// Once the timer has expired change wifi_button_timeout flag to allow the master system to reenter deep sleep.

void v_read_buttons(void *parameters) 
{
  wifi_button_timeout = false; // for now, stop the system from entering deep sleep
  
  unsigned long button_debounce_counter = millis() + 2000;   // Set the variable to 2000ms ahead of millsi()


  while (1) 
  {
    if (!wifi_button_timeout)  // Check to see if the time flag is still false
    {
      if (millis() >= button_debounce_counter)
                         wifi_button_timeout = true;  // Allow a deep sleep to continue  
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Set this to fixed delay for 1000ms. The top will check the progress and if an update is needed.
  }
}


