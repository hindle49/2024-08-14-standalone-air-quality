void vReadAirQuality(void *parameters) 
{
  bool air_quality_module_started = false;  // Use this to flag if the two parts of the air quality module have been started

  unsigned int warm_up_delay = HOW_LONG_TO_WARM_UP + millis();  // Set the variable to a time in the future, to be used to hold off the measurements.  

  sensors_event_t te;
  sensors_event_t hu;


 while (1) // Just to keep the RTOS task running
  {
   
   if( air_quality_module_started == false ) //false indicates the two air quality parameters have not been started
       {
        aht.begin();
        ens160.begin();
        
        //ens160.setMode(ENS160_OPMODE_LP);
        ens160.setMode(ENS160_OPMODE_STD);

        //vTaskDelay(5000 / portTICK_PERIOD_MS);  // A short sleep until both units have started 5000ms. (5 second)
        air_quality_module_started = true;      // Flag that the two modules have started, so there is no attempt to restart them again.
        
        debugln("Started both air quality modules");
       }

    debugln("Read modules ");

    // Read the temp and humidity first, before the air measurement heaters have an effect.
    
    if ( temp_hum_acquired == false )  // need the temperature and humidity module data
        {
          //change the flag to show data recieved
          aht.getEvent(&hu, &te);  

          temp_hum_acquired = true; // flag that the temperature and humidity has been acquired

          temperature = te.temperature;
          humidity    = hu.relative_humidity;
        
          debug("Humidity : "); 
          debug(humidity);
          debug("  Temperature : ");
          debugln(temperature);

          temp_hum_acquired = true; // flag that the temperature and humidity has been acquired (moved to after the variables have been set)

        }

    
    if (millis() >= warm_up_delay )  // check to see if millis() has over taken the set time in warm_up_delay 
      {
      if ( (air_quality_acquired == false) && ens160.available() )  // tests to see if the data has already been acquired and if there is new data to get from the module
         {
          ens160.measure(true);
          ens160.measureRaw(true);

          Aqi = ens160.getAQI();
          Tvoc = ens160.getTVOC();
          Co2 = ens160.geteCO2();
          Hp0 = ens160.getHP0();
          Hp1 = ens160.getHP1();
          Hp2 = ens160.getHP2();
          Hp3 = ens160.getHP3();

          ens160.setMode(ENS160_OPMODE_DEP_SLEEP);

          debug("AQI : ");
          debugln(Aqi);

          debug("TVOC : ");
          debugln(Tvoc);

          debug("Co2 : ");
          debugln(Co2);

          debug("HP0 : ");
          debugln(Hp0);

          debug("HP1 : ");
          debugln(Hp1);

          debug("HP2 : ");
          debugln(Hp2);

          debug("HP3 : ");
          debugln(Hp3);        

          //change the flag to show data recieved
          air_quality_acquired = true;  // flag that the air quality data has been acquired (good that this is after the variables have been set)
        }
      }  



    
    if ( ( air_quality_acquired == true) && (temp_hum_acquired == true)) vTaskDelete(NULL);
    
    // wait for 2 seconds before trying again
    vTaskDelay(2000 / portTICK_PERIOD_MS);  // A short sleep until both sets of data acquired ms. (2 second)

  } // End of the while loop
}     // End of the function
