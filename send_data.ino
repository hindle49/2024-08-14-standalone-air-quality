void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  //debug("\r\nLast Packet Send Status:\t");
  //debugln(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



void v_send_data(void *parameters) {
  unsigned long forced_resend_data_timeout = millis() + 120000;  //  2 * 60 * 1000   2 minutes;
  bool keep_alive = false;

  while (1) {

    if (millis() >= forced_resend_data_timeout) {      // Check to see if the forced resend has expired
      send_data_now = true;                            // for the resent
      keep_alive = true;
      forced_resend_data_timeout = millis() + 240000;  // reset the time out counter
    }

    if (send_data_now == true) {
      debugln("Sending data");
      // First clear the flag above
      
      if (!keep_alive)  // only copy the structures if it is not a keep alive
      {
      sent = local;        // Copy the entire structures from local to sent
      }
      
      keep_alive = false;
      send_data_now = false;

      sent.new_data_flag = true;  //make sure the send flag is set

      //Calulate the checksup
      sent.checksum = sent.pattern + sent.step + sent.sync + sent.brightness;

      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(lantern_1_Address, (uint8_t *)&sent, sizeof(sent));
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);  //300ms delay, before looping again

    
  } // End of the while(1) loop
}