#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
#include "constants.h"

#include <ESP32WebServer.h>
#include <ESPmDNS.h>

ESP32WebServer server(80);

#include "pins.h"
#include <ESP32WebServer.h>
#include <ESPmDNS.h>
#include "esp_wps.h"
//#include "OTAServer.h"
#include "handleSerial.h"
//#include <ArduinoOTA.h> //added by ardy

void setup() {
  //Check Battery Voltage pin voltage = 1/2 batt voltage
  vBat = (analogRead(batPin)*3.30*2)/4096;

  //Labels the power state of the battery as Low or Ok
  if (vBat< uBAT_LOW_THRESHOLD) {uPOWER_STATE = uBAT_LOW ;}
  else{
    uPOWER_STATE = uBAT_OK;
  }

  if (uPOWER_STATE) {
    Serial.begin(115200);
    SerialBT.begin( devID ); //Bluetooth device name

  }

  //LEDc Driver Setup
  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);
  ledcSetup(2, freq, resolution);
  ledcSetup(3, freq, resolution);

  //Makes pins a variable
  ledcAttachPin(LED1, 0);
  ledcAttachPin(LED2, 1);
  ledcAttachPin(LED3, 2);
  ledcAttachPin(LED4, 3);

  ledcWrite(0, 256); // Off Side
  ledcWrite(1, 256); // Off Side
  ledcWrite(2, 256); // Off Side
  ledcWrite(3, 256); // Off Side


}

void loop() {
  vBat = (analogRead(batPin)*3.30*2)/4096; // getting battery voltage

  // checking if battery life is above voltage threshold
  if (vBat> uBAT_LOW_THRESHOLD) {

    uPOWER_STATE = uBAT_OK;}
    else{

      uPOWER_STATE = uBAT_LOW;}

      if (uPOWER_STATE==uBAT_OK) {
        // Check if execution time surpasses sampling frequencies
        // This is done to ensure data is not lost from delays

        if ( ( millis()-loopStart) >=samplingFrequency) {

          loopStart = millis(); // measures the execution time
          for (size_t i = 0; i <= 4; i++) {

            //1.read dark current first
            ledcWrite(i, 256); // Off Side
            delay(onDelay);

            darkCH1 = analogReadFunction(dly_avg, n_reads,CH1);//analogRead(CH1);
            darkCH2 = analogReadFunction(dly_avg, n_reads,CH2);
            darkCH3 = 0; //dummy value
            darkCH4 = 0; //dummy value

            //2.read LED on now. depending on i (var. for wavelenngth and LED) .
            if (i!=4) {
              ledcWrite(i, intensity[i]);   // On Side
            }
            delay(onDelay);

            onCH1 = analogReadFunction(dly_avg, n_reads,CH1);
            onCH2 = analogReadFunction(dly_avg, n_reads,CH2);
            onCH3 = 17;
            onCH4 = 27;

            //3.subtract on_values by dark_values
            valCH1 = onCH1 - darkCH1;
            valCH2 = onCH2 - darkCH2;
            valCH3 = onCH3 - darkCH3;
            valCH4 = onCH4 - darkCH4;

            ledcWrite(i, 256); // Off Side

            // prints results of detectors
            sprintf(bufferSerial, formatString ,i,valCH1,valCH2,valCH3,valCH4,vBat,millis()/10000.00); //Convert to 4 channels
            SerialBT.println(bufferSerial);
          }

          handleSerial();
        }
      }
      else{
        //Send End of Connection message to data stream
        // Error message to show the battery power is not enough
        sprintf(bufferSerial,formatString,0,999,999,999,999,vBat,millis()/10000.0); //Convert to 4 channels
        delay(100);
        sprintf(bufferSerial,formatString,0,999,999,999,999,vBat,millis()/10000.0);
        delay(100);
        sprintf(bufferSerial, formatString,0,999,999,999,999,vBat,millis()/10000.0);
        delay(100);
        sprintf(bufferSerial, formatString,0,999,999,999,999,vBat,millis()/10000.0);
        delay(100);

        btStop(); //Stop BluetoothSerial

        while (uPOWER_STATE == uBAT_LOW_THRESHOLD) {
          // This flashes the LED and sets the ESP32 to sleep until battery power is restored
          ledcWrite(0, intensity[0]);   // On State
          delay(100);
          ledcWrite(0, 256); // Off State

          delay(100);

          ledcWrite(0, intensity[0]);   // On Side
          delay(500);
          ledcWrite(0, 256); // Off Side

          esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
          esp_deep_sleep_start();

          vBat = (analogRead(batPin)*3.30*2)/4096;

          if (vBat>3.5) {uPOWER_STATE = uBAT_OK;}
        }
        esp_restart();
      }

    }

//*******************************************//
//******** Self-made functions **************//
//*******************************************//

//******** Function to read multiple values and average them **************//
// inputs: All the specific values are initialized in the beginning
// 1. dly_avg (in us) = Duration of the delay per detector read.
// 2. n_reads         = number of times detectors are being read. e.g. n_reads = 5 --> the detetctors is averaging between 5 values
// 3. sensorPin       = Which pin are you reading. As each time this function is called, different pin inputs are inserted (in the main loop)

int analogReadFunction(const int dly_avg, const int n_reads, const int sensorPin)
{
  int sensorValue = 0;
  for (int i = 0; i <= n_reads; i++)
  {
    delayMicroseconds(dly_avg);
    if (i >= 1)
    {
      sensorValue = analogRead(sensorPin) + sensorValue;
    }
    //sensor1 = far; sensor2 = near
  }
  return sensorValue / (n_reads);
}
