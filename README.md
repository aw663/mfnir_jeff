# minature-fNIR

##DRAFT!

The presented device is a proof of concept technology of a miniaturized, functional near infrared (fNIR) platform for small animal models such as rats, mice, or rabbits. fNIR is a non-invasive method to record the metabolic activity of neural tissue which uses red and infrared light to measure the oxygenation change of tissue. The platforms can be used to wirelessly, monitor the hemodynamic response of interhemispheric change during surgically or pharmaceutically induced stroke while allowing the animal to move freely.

## Hardware

### Head Probe
The developed device consists of two physical components: the head probe photosensor array and controller module. Photosensor array is composed of two dual wavelength  [730nm and 850nm] LED sources and four photodiodes. A set of two photodiodes and one dual wavelength light source monitors the metabolic activity of one hemisphere of the brain. Light intensity, current signal from the photodiodes is converted to a voltage signal, amplified and filtered using a low-pass transimpedance amplifier. The photodiodes are aligned with the Bregma and placed laterally 4.25 mm from the midline on either side. The photodiodes are placed 6.2 mm and 9.5 mm back from the Bregma, inline with the LED as this will give the desired penetration depth of 3.1 mm and 4.75mm.

![Alt text](imgs/headprobe.png?raw=true "Head probe")

#### Dimensions

![Alt text](imgs/head_pcb.PNG?raw=true "Head probe PCB")

### Controller & Communication Unit
The microcontroller subsystem is responsible for light source control, second stage analog processing, and analog-to-digital conversion, and wireless transmission to host software. The microcontroller uses low-power a voltage of 3.3V and an average current consumption of 130 mAh.

![Alt text](imgs/pcb.PNG?raw=true "Controller PCB")

## Software


### Firmware



Dependencies (Libraries)

[BluetoothSerial](https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial)

[ESP32WebServer]

[ESPmDNS]

[Update]

[esp_wps]

[OTAServer]

### Host software
![Alt text](imgs/gui.PNG?raw=true "GUI")

Dependencies
```
LabVIEW 2014 Run-Time Engine (http://www.ni.com/download/labview-run-time-engine-2014/4889/en/) 
TDM Excel Add-In for Microsoft Excel (http://www.ni.com/example/27944/en/)
```

## Development Environment 
Atom 1.31.2 x64

LabVIEW 2014
