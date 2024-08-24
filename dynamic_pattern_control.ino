void dynamic_pattern_controller()
  {
    static uint8_t       pattern_number_counter = 0; // Use this to step through to the next pattern in the define sequence
    static unsigned long pattern_duration_timer = 0;

    switch(day_state)  // This was using on the system_mode, which should never have done anything. Should be looking at the day_state
    {
      // ------------------------------------------------
      case NIGHT:
          if ( (system_mode_transition == true) || (day_state_transistion == true ))  // this has just happend so act on the change
                 {
                 local.pattern    = 0x00;
                 local.step       = 0xff;
                 local.sync       = 0x00;
                 local.brightness = 0x00;
                 local.checksum   = 0x00;
                 send_data_now    = true;
                 }
      break;
      // ------------------------------------------------
      case EARLY_MORNING:
         if ( (system_mode_transition == true) || (day_state_transistion == true ) )  // this has just happend so act on the change
                 {
                 local.pattern    = 0xf7; // All white
                 local.step       = 0xff;
                 local.sync       = 0x00;
                 local.brightness = 0x01;
                 local.checksum   = 0x00;
                 send_data_now    = true;
                 }

      break;
      // ------------------------------------------------
      case MORNING:
      // This doesn't need to check if there's been a recent transistion, as the function continues all the time
           if ( (system_mode_transition == true) || (millis() >= pattern_duration_timer) )
             {
             local.pattern          =  patterns[pattern_number_counter][0];
             local.step             =  patterns[pattern_number_counter][1];
             local.sync             =  patterns[pattern_number_counter][2];
             local.brightness       =  patterns[pattern_number_counter][3];
             pattern_duration_timer = (patterns[pattern_number_counter][4] * 1000) + millis();
             pattern_number_counter++; // select next pattern

            if (pattern_number_counter >= number_of_patter_steps) pattern_number_counter = 0;
            send_data_now = true;

            }
            display_time_varable =  ( pattern_duration_timer - millis() ) / 1000; 
      break;
      // ------------------------------------------------
      case DAY:
           if ( (system_mode_transition == true) || (day_state_transistion == true ) )  // this has just happend so act on the change
                 {
                 local.pattern    = 0x00;
                 local.step       = 0xff;
                 local.sync       = 0x00;
                 local.brightness = 0x00;
                 local.checksum   = 0x00;
                 send_data_now    = true;
                 }
      break;
      // ------------------------------------------------
      case EARLY_EVENING:
            if ( (system_mode_transition == true) || (day_state_transistion == true ) )  // this has just happend so act on the change
                 {
                 local.pattern    = 0xf7; // All white
                  local.step      = 0xff;
                 local.sync       = 0x00;
                 local.brightness = 0x01;
                 local.checksum   = 0x00;
                 send_data_now    = true;
                 }
      break;
      // ------------------------------------------------
      case EVENING:
           // This doesn need to check if there's been a recent transistion, as the function continues all the time
           if( (system_mode_transition == true) || (millis() >= pattern_duration_timer) )
             {
             local.pattern          =  patterns[pattern_number_counter][0];
             local.step             =  patterns[pattern_number_counter][1];
             local.sync             =  patterns[pattern_number_counter][2];
             local.brightness       =  patterns[pattern_number_counter][3];
             pattern_duration_timer = (patterns[pattern_number_counter][4] * 1000) + millis();
             pattern_number_counter++; // select next pattern

            if (pattern_number_counter >= number_of_patter_steps) pattern_number_counter = 0;
            send_data_now = true;

            }
            display_time_varable =  ( pattern_duration_timer - millis() ) / 1000; 
      break;
      // ------------------------------------------------
    } //End of the switch

  }