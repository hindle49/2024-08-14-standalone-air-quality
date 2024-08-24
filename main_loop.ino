void vmain_loop (void *parameters)

{
  while(1)
  {
    #ifdef debug
     if (buttonState[0] == false) ESP.restart();   //force a reset
    #endif


    // Step 2 define whaich parts of the day the system is in
    
    if   ( (day_state != NIGHT)         &&         (tm_hour <= 5 )                      )  { day_state = NIGHT;         day_state_transistion = true; }       // midnight till 5
    if   ( (day_state != EARLY_MORNING) &&         (tm_hour == 6 )                      )  { day_state = EARLY_MORNING; day_state_transistion = true; }       // 6 to 7  - one hour
    if   ( (day_state != MORNING)       &&       ( (tm_hour >= 7 ) && (tm_hour <= 8)  ) )  { day_state = MORNING;       day_state_transistion = true; }       // 7 to 9  - two hours
    if   ( (day_state != DAY)           &&       ( (tm_hour >= 9 ) && (tm_hour <= 13) ) )  { day_state = DAY;           day_state_transistion = true; }       // 9 until 2pm
    if   ( (day_state != EARLY_EVENING) &&         (tm_hour == 14)                      )  { day_state = EARLY_EVENING; day_state_transistion = true; }       // 2pm to 3pm - one hour
    if   ( (day_state != EVENING)       &&       ( (tm_hour >= 15) && (tm_hour <= 22) ) )  { day_state = EVENING;       day_state_transistion = true; }       // 3pm to 11pm
    if   ( (day_state != NIGHT)         &&         (tm_hour >= 23)                      )  { day_state = NIGHT;         day_state_transistion = true; }       // 23 to midnight 

    if  (   // This is messy. If the node is ON, red LED on.  If the mode is auto, the red LEd will be on at the day_state conditions.
        (system_mode == ON ) || 
          ( 
          (system_mode == AUTO) && 
                    ( (day_state == EARLY_MORNING) || (day_state == MORNING) || (day_state == EARLY_EVENING) ||(day_state == EVENING) ) 
          )
        )
          digitalWrite(RED_LED, HIGH);
    else
          digitalWrite(RED_LED, LOW);
    
    // Step 3 - Set the mode. i.e. Off/ Auto / On
    if (  ( buttonState[2] == false ) && ( button1JustChanged[2] == true) )  // button 2 just pressed
      {
        system_mode++;             // next mode in the list
        if (system_mode == STOP)  system_mode = OFF;   // STOP isn't correct here, so reset to OFF
        system_mode_transition = true;                 // flag a change of mode
        button1JustChanged[2] = false;                 // reset the transition flag

        if(system_mode == AUTO)   //green led on if in auto mode
          digitalWrite(GREEN_LED, HIGH);
        else
          digitalWrite(GREEN_LED, LOW);
      }

    // Step 4
    switch(system_mode)
     {
     case OFF:
             if (system_mode_transition == true )  // this has just happend so act on the change
                {
                system_mode_transition = false;  
                local.pattern    = 0x00;
                local.step       = 0xff;
                local.sync       = 0x00;
                local.brightness = 0x00;
                local.checksum   = 0x00;
                send_data_now    = true;
                } 
              break;
      case ON:
              if ( system_mode_transition == true )  // this has just happend so act on the change
                 {
                 system_mode_transition = false;    // For testing, this is set to candle effect mode
                 local.pattern    = 0x60;
                 local.step       = 0x00;
                 local.sync       = 0x00;
                 local.brightness = 0x00;
                 local.checksum   = 0x00;
                 send_data_now    = true;
                 }
              break;
        case AUTO:
          {
            dynamic_pattern_controller();
            system_mode_transition = false;
            day_state_transistion  = false;
            break;
          }


     }
    vTaskDelay(100 / portTICK_PERIOD_MS);  // 100mS
  } // end of the while lop

}