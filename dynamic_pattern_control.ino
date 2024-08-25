void dynamic_pattern_controller()
  {
    static uint8_t       pattern_number_counter = 0; // Use this to step through to the next pattern in the define sequence
    static unsigned long pattern_duration_timer = 0;

    switch(day_state)  // This was using on the system_mode, which should never have done anything. Should be looking at the day_state
    {
      // ------------------------------------------------
      case NIGHT:
      break;
      // ------------------------------------------------
      case EARLY_MORNING:
      break;
      // ------------------------------------------------
      case MORNING:
      break;
      // ------------------------------------------------
      case DAY:
      break;
      // ------------------------------------------------
      case EARLY_EVENING:
      break;
      // ------------------------------------------------
      case EVENING:
 
      break;
      // ------------------------------------------------
    }

  }