// The global paramter  'averageLux' will hold the measured light level


void vreadAirQuality(void *parameters) 
{
 for(;;) // Just to keep the RTOS task running
 {
   if( air_quality_acquired == false) //false indicates the two air quality parameters have not been acquired
    {

    // when both paramter sets have been acquired   air_quality_acquired == false
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // A short sleep until both sets of data acquired ms. (1 second)
    }
   else
   {
    // There's no need to do anything, so set a long vTaskDelay
   vTaskDelay(60000 / portTICK_PERIOD_MS);  // A short sleep until both sets of data acquired ms. (60 seconds)
   }


 }


}
