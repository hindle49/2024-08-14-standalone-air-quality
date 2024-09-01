/*
void v_read_buttons(void *parameters) {
  while (1) {
    static unsigned long lastDebounceTime[4] = { 0, 0, 0, 0 };
    static unsigned long debounceDelay[4] = { 50, 50, 50, 50 };
    static bool          lastButtonState[4] = { true, true, true, true };
    bool reading[4];

    // read the state of the switches into local variables:
    reading[0] = digitalRead(BUTTON_1);

    
    for (uint8_t i = 0; i <= 3; i++) {
      // reset the flag which indicates there has been a recent change
      button1JustChanged[i] = false;

      // check to see if you just pressed the button
      // (i.e. the input went from HIGH to LOW), and you've waited long enough
      // since the last press to ignore any noise:

      // If the switch changed, due to noise or pressing:
      if (reading[i] != lastButtonState[i]) lastDebounceTime[i] = millis();  // reset the debouncing timer1. This happens every time the last and present reading are NOT the same


      if ((millis() - lastDebounceTime[i]) > debounceDelay[i])  // The first time this runs lastDebounceTime1 will be zero. Therefore, the boolan test will be true.
      {                                                         // and the function will run. This test will be true for the majority of the time. It will only fail during a debounce time out.
        //Serial.println("button bounce over flow");
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading[i] != buttonState[i])  //remeber this will be tested during a stable (none change time). Only when an button overflow has occured and the
        {                                  //previous button states are different should this run.



          // only toggle the LED if the new button state is HIGH
          buttonState[i] = reading[i];  // as there has been effect, align the flags
          debug  ("Button Change ");
          debug  (i);
          debug  (" ");
          debugln(buttonState[i]);

          button1JustChanged[i] = true;
        }
      }
      lastButtonState[i] = reading[i];  // This is to flag the status of this test, so it can be used in the nexy cycle, as the last status.
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);  // Set this to fixed delay for 50ms. The top will check the progress and if an update is needed.
  }
}

*/
