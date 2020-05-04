# LINX - Digilent/LabVIEW MakerHub for ESP32

http://www.labviewmakerhub.com/linx

## Overview

ESP32 support for LINX plug-ins in LabVIEW.
ESP32 information can be used in LabVIEW via Wi-Fi or Serial.

## Sample code

```
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <LinxESP32.h>
#include <LinxESP32WifiListener.h>

LinxESP32* LinxDevice;

void setup()
{
  LinxDevice = new LinxESP32();
  LinxWifiConnection.Start(LinxDevice, 44300);
}

void loop()
{
  LinxWifiConnection.CheckForCommands();
  delay(1);
}
```
