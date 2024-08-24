// The global paramter  'averageLux' will hold the measured light level


void vreadLightLevel(void *parameters) {
    static uint8_t instanceCounter = 0;  // To be used to count through the instances of light in the averaging calculation. i.e. 0 to 60 instances
    uint16_t luxSamples[60];  // stores samples of light level which are then averaged into averageLux 

    for (uint8_t i = 0; i < 60; i++) 
    {
      luxSamples[i] = 100;
    }

  
  while (1) {



    if (instanceCounter >= 60) instanceCounter = 0;    // 60 samples will be four samples per minute, so an average over 15 minutes
    luxSamples[instanceCounter] = LightSensor.GetLightIntensity();
    //debug("raw light level ");
    //debugln(LightSensor.GetLightIntensity());
    instanceCounter++;

    averageLux = 0.0;
    for (uint8_t i = 0; i < 60; i++) {
      averageLux = averageLux + (long)luxSamples[i];
    }
    
    averageLux = averageLux / 60;
    
    debug("Reading light levels: averageLux = ");
    debugln(averageLux);
    vTaskDelay( (1000 * 5) / portTICK_PERIOD_MS);  // Sample ever 5 seconds
  }
}
