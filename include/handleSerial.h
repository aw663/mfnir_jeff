#include <Arduino.h>

void handleSerial(){
  if (SerialBT.available() > 0) {

      char received = SerialBT.read();

      //Serial.print("REC: ");
      //Serial.print(received);
      inData += received;
      // Process message when new line character is recieved
      if (received == '\n') {
        String function = inData.substring(0, 2);
        cmdOK = 0;

        //-----CMD TO PUMP SPEED-------------------------------------------------
        //-----------------------------------------------------------------------
        if (function == "ID") {  //Update

          SerialBT.print("EID ");
          SerialBT.println(devID);
          cmdOK = true;
        }
        if (function == "UP")
            {
            //  OTAServer();
              cmdOK = true;
            }
      if (function == "WF")
          {
            int WFstart = inData.indexOf("WF");
            char WFval[sizeof(inData.substring(WFstart + 2))];
            inData.substring(WFstart + 2).toCharArray(WFval,    sizeof(WFval));

            password = WFval;

            SerialBT.print("EWF ");
            SerialBT.println(password);
            cmdOK = true;
          }

        if (function == "PW")
            {
              int PWstart = inData.indexOf("PW");
              char PWval[sizeof(inData.substring(PWstart + 2))];
              inData.substring(PWstart + 2).toCharArray(PWval,    sizeof(PWval));

              ssid = PWval;

              SerialBT.print("EPW ");
              SerialBT.println(ssid);
              cmdOK = true;
            }

        if (function == "IS") {

          int ISstart = inData.indexOf("IS");
          int ISval = inData.substring(ISstart + 2).toInt(); //extracts data from command packet

         intensity[0]=256-ISval;
         intensity[1]=256-ISval;
         intensity[2]=256-ISval;
         intensity[3]=256-ISval;

          SerialBT.print("EIS ");
          SerialBT.println(ISval);
          cmdOK = true;
          }
        inData = ""; // Clear recieved buffer
        if (!cmdOK) {
          SerialBT.println("NOK");
        }
        }
      }
  }
//}
