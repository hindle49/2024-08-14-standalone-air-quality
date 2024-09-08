void v_ReadBatteryVoltage(void *parameters)  // The is the main display setting am to run once a second
{
  
  int temp_battery_voltage  = 0; 
  while (1) 
  {
   analogReadResolution(13); // was 12

   temp_battery_voltage = analogReadMilliVolts(VBATT_PIN);

     
   battery_voltage = (float)(temp_battery_voltage * 2.0);  // This is because the voltage on the pin is half the actual battery voltage.

   battery_voltage = battery_voltage / 1000; // This is because the the reading is in milli voltages, so convert to volts. In the OLED trunk to one decial place.

   if (battery_voltage < BATTERY_END_VOLTAGE) // Check to see if the system should be stopped
        {

        }




   vTaskDelete(NULL);  // Kill this task, no need for it to loop again.

  }
}