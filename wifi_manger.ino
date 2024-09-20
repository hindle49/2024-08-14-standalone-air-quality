void v_Wifi_connection(void *parameters)  // The is the main display setting am to run once a second
{

 
 while (1) 
   {
     WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
    
    if (request_new_WiFi)  // test to see if the flag is set to ask for new wifi
      {
        wm.resetSettings(); // wipe settings
        request_new_WiFi = false; // stop the request from happing again
      }

    if(wm_nonblocking) wm.setConfigPortalBlocking(false);

    // add a custom input field
    int customFieldLength = 40;

    // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\"");
  
    // test custom html input type(checkbox)
    // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\" type=\"checkbox\""); // custom html type
  
    // test custom html(radio)
    const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
    new (&custom_field) WiFiManagerParameter(custom_radio_str); // custom html input
  
    wm.addParameter(&custom_field);
    //wm.setSaveParamsCallback(saveParamCallback);

    // custom menu via array or vector
    // 
    // menu tokens, "wifi","wifinoscan","info","param","close","sep","erase","restart","exit" (sep is seperator) (if param is in menu, params will not show up in wifi page!)
    // const char* menu[] = {"wifi","info","param","sep","restart","exit"}; 
    // wm.setMenu(menu,6);
    std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
    wm.setMenu(menu);

    // set dark theme
    wm.setClass("invert");

    //set static ip
    // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // set static ip,gw,sn
    // wm.setShowStaticFields(true); // force show static ip fields
    // wm.setShowDnsFields(true);    // force show dns field always

    // wm.setConnectTimeout(20); // how long to try to connect for before continuing
    wm.setConfigPortalTimeout(90); // auto close configportal after n seconds
    // wm.setCaptivePortalEnable(false); // disable captive portal redirection
    // wm.setAPClientCheck(true); // avoid timeout if client connected to softap

    // wifi scan settings
    // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
    // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
    // wm.setShowInfoErase(false);      // do not show erase button on info page
    // wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons
  
    // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("Air Quality","password"); // password protected ap

    if(!res) 
      {
      WiFi_Connected = false;
      debugln("Failed to connect or hit timeout");
      // ESP.restart();
      } 
    else 
      {
      //if you get here you have connected to the WiFi    
      WiFi_Connected = true;
      debugln("connected...yeey ");
      //WIFI_SSID = wm.getSSID();
      debugln (WiFi.SSID() );
      //ESP.restart();
      }
    vTaskDelay(1000 / portTICK_PERIOD_MS);  //300ms delay, before looping again
  } // End of while

} // end of function
