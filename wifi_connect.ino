/*
void v_wifi_connection(void *parameters)
{
  for(;;)
   {
   if (WiFi.status() == WL_CONNECTED)  // First check the present wifistatus
      {
      WiFi_Connected = true; // WiFi connection established
      }  
    else
      {
      WiFi_Connected = false; 
      previous_connection_state = false;    //*********** move this to the MQTT connection later
      }
   
    if  (WiFi_Connected == false)  // Now do somthing becuse the Wifi has been found not to be connected
      {
      // Thinking this through, this will be called if there's still no Wifi connection.
      // Move on the SSID point and reconnect.
      WiFi_Pnt++;                                // Move the pointer forward.
      if (WiFi_Pnt > WiFi_Defs) {WiFi_Pnt = 1;}  // If over run, reset back

      switch(WiFi_Pnt)  // Now assign the required pointer for SSID and password
         {
         // define a case for each device connection to be attempted
         case 1: WIFI_SSID = WiFi_SSID1; WIFI_PWD = WiFi_PWD1; break;
         case 2: WIFI_SSID = WiFi_SSID2; WIFI_PWD = WiFi_PWD2; break;
         case 3: WIFI_SSID = WiFi_SSID3; WIFI_PWD = WiFi_PWD3; break;
         case 4: WIFI_SSID = WiFi_SSID4; WIFI_PWD = WiFi_PWD4; break;
         case 5: WIFI_SSID = WiFi_SSID5; WIFI_PWD = WiFi_PWD5; break;
         }

      WiFi.disconnect();                      // terminate the current connection if made
      vTaskDelay(5000 / portTICK_PERIOD_MS);  // release the task for 5 second
      
      WiFi.mode (WIFI_STA);             // define wifi client
      WiFi.begin(WIFI_SSID, WIFI_PWD);

      debug   ("trying wifi ssid  ");
      debugln (WIFI_SSID);
      
      vTaskDelay(15000 / portTICK_PERIOD_MS);  // release the task for 15 second, wait for a connection
      }
    vTaskDelay(5000 / portTICK_PERIOD_MS);  // release the task for 5 second before testing if the connection is still working 
   }
}

*/
