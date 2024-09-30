// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       debugln(mqtt.connectErrorString(ret));
       debugln("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}


void v_send_data(void *parameters)
 {
  while(1)
    {
    if ( (air_quality_acquired == true) && (temp_hum_acquired == true) && (WiFi_Connected == true))  // if the three flags are true then continue with the data, else wait
      {
      MQTT_connect(); // 
      if (mqtt.connected()) 
        {
        item_0.publish(temperature);
        item_1.publish(humidity);
        item_2.publish((float)Aqi);
        item_3.publish((float)Tvoc);
        item_4.publish(Co2);
        item_5.publish(Hp0);
        item_6.publish(Hp1);
        item_7.publish(Hp2);
        item_8.publish(Hp3);
        item_9.publish(battery_voltage);

        vTaskDelay(1000 / portTICK_PERIOD_MS);  //2000ms delay, before looping again

        data_sent_to_brocker = true;

        vTaskDelete(NULL);  // Kill this task, no need for it to loop again.

        }
      }

    vTaskDelay(2000 / portTICK_PERIOD_MS);  //2000ms delay, before looping again
    }
}

