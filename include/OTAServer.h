void OTAServer(){
  WiFi.begin(ssid, password);
  delay(1000);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(500);
    SerialBT.print(".");
  }

//  SerialBT.println("Connection Success");
  if (!MDNS.begin(host)) {
  //  SerialBT.println("Error setting up MDNS responder!");
    delay(5000);
    ESP.restart();
  }

  SerialBT.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, [](){
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, [](){
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
    esp_wifi_wps_disable(); ESP.restart();
  },[](){
    HTTPUpload& upload = server.upload();
    if(upload.status == UPLOAD_FILE_START){
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if(!Update.begin(UPDATE_SIZE_UNKNOWN)){//start with max available size
        Update.printError(Serial);
      }
    } else if(upload.status == UPLOAD_FILE_WRITE){
      /* flashing firmware to ESP*/
      if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
        Update.printError(Serial);
      }
    } else if(upload.status == UPLOAD_FILE_END){
      if(Update.end(true)){ //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
  while(1){
    server.handleClient();
    delay(1);
  }
}
