bool getNTPtime(int sec) 
 {
 {
    uint32_t start = millis();
    do 
    {
      time(&now);
      localtime_r(&now, &timeinfo);
      //Serial.print(".");
      vTaskDelay(10 / portTICK_PERIOD_MS);  // wait 10ms and try again
    } 
    
    while (  (  (millis() - start) <= (1000 * sec)  ) && (timeinfo.tm_year < ( 2016 - 1900  )   )    );
    
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    
    //Serial.print("now ");  Serial.println(now);
    
    char time_output[30];
    
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
    #ifdef DEBUG
      Serial.println(time_output);
      Serial.println();
    #endif
    
    // copy time to exRTP

    //myRTC.setEpoch(now, false);
    
    myRTC.setYear   (timeinfo.tm_year - 100 ); // Copy year to exRTC
    myRTC.setMonth  (timeinfo.tm_mon + 1);     // Copy month to exRTC
    myRTC.setDate   (timeinfo.tm_mday);        // Copy date    to exRTC
    myRTC.setDoW    (timeinfo.tm_wday );       // Copy day of week toexRTV
    myRTC.setHour   (timeinfo.tm_hour);        // Copy hours   to exRTC
    myRTC.setMinute (timeinfo.tm_min);         // Copy minute  to exRTC
    myRTC.setSecond (timeinfo.tm_sec);         // Copy seconds to exRTC
   

  }
  return true;


 }


void vget_new_unix_time (void *parameters)

{
  configTime(0, 0, NTP_SERVER);
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", TZ_INFO, 1);
  
  while(1)
     {

           
   vTaskDelay(1000 / portTICK_PERIOD_MS);  // wait 1s and try again
     }
}