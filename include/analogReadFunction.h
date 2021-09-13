#include <Arduino.h>

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
