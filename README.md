# LINX - Digilent/LabVIEW MakerHub for ESP32

http://www.labviewmakerhub.com/linx

## Overview

ESP32 support for LINX plug-ins in LabVIEW.
ESP32 information can be used in LabVIEW via Wi-Fi or Serial.

## Channel List
| Channel | Digital Input | Digital Outout | PWM & Servo    | Analog Input |
|---------|---------------|----------------|----------------|--------------|
| 0       | GPIO0         | GPIO0          | Any Output Pin |              |
| 1       | GPIO1         | GPIO1          | Any Output Pin |              |
| 2       | GPIO2         | GPIO2          | Any Output Pin |              |
| 3       | GPIO3         | GPIO3          | Any Output Pin |              |
| 4       | GPIO4         | GPIO4          | Any Output Pin |              |
| 5       | GPIO5         | GPIO5          | Any Output Pin |              |
| 6       | GPIO6         | GPIO6          | Any Output Pin |              |
| 7       | GPIO7         | GPIO7          | Any Output Pin |              |
| 8       | GPIO8         | GPIO8          | Any Output Pin |              |
| 9       | GPIO9         | GPIO9          | Any Output Pin |              |
| 10      | GPIO10        | GPIO10         | Any Output Pin |              |
| 11      | GPIO11        | GPIO11         | Any Output Pin |              |
| 12      | GPIO12        | GPIO12         | Any Output Pin |              |
| 13      | GPIO13        | GPIO13         | Any Output Pin |              |
| 14      | GPIO14        | GPIO14         | Any Output Pin |              |
| 15      | GPIO15        | GPIO15         | Any Output Pin |              |
| 16      | GPIO16        | GPIO16         |                |              |
| 17      | GPIO17        | GPIO17         |                |              |
| 18      | GPIO18        | GPIO18         |                |              |
| 19      | GPIO19        | GPIO19         |                |              |
| 20      |               |                |                |              |
| 21      | GPIO21        | GPIO21         |                |              |
| 22      | GPIO22        | GPIO22         |                |              |
| 23      | GPIO23        | GPIO23         |                |              |
| 24      |               |                |                |              |
| 25      | GPIO25        | GPIO25         |                |              |
| 26      | GPIO26        | GPIO26         |                |              |
| 27      | GPIO27        | GPIO27         |                |              |
| 28      |               |                |                |              |
| 29      |               |                |                |              |
| 30      |               |                |                |              |
| 31      |               |                |                |              |
| 32      | GPIO32        | GPIO32         |                | GPIO32       |
| 33      | GPIO33        | GPIO33         |                | GPIO33       |
| 34      | GPIO34        |                |                | GPIO34       |
| 35      | GPIO35        |                |                | GPIO35       |
| 36      | GPIO36        |                |                | GPIO36       |
| 37      | GPIO37        |                |                | GPIO37       |
| 38      | GPIO38        |                |                | GPIO38       |
| 39      | GPIO39        |                |                | GPIO39       |

## Sample code

### Wi-fi
```c
#include <LinxESP32.h>

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

### BluetoothSerial
```c
#include <LinxESP32.h>

LinxESP32* LinxDevice;

void setup()
{
  LinxDevice = new LinxESP32();
  LinxSerialConnection.Start(LinxDevice, 3);
}

void loop()
{
  LinxSerialConnection.CheckForCommands();
  delay(1);
}
```
