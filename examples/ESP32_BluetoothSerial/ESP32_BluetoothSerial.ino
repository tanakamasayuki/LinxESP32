/****************************************************************************************
**  This is example LINX firmware for use with the ESP32 device
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**
**  Written By Ken Sharp
**
**  BSD2 license.
****************************************************************************************/

//Include Device Specific Header From Sketch>>Import Library (In This Case LinxESP32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxESP32.h>

//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxESP32* LinxDevice;

//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxESP32();

  // PWM & Servo Setup
  uint8_t pwmList[] = {};         // Max16Ch {0, 26, ...}
  uint16_t pwmFrequency = 12000;  // LED:12000, Servo:50
  for (int i = 0; i < sizeof(pwmList); i++) {
    ledcSetup(i, pwmFrequency, 8);
    pinMode(pwmList[i], OUTPUT);
    ledcAttachPin(pwmList[i], i);
  }

  // I2C Begin ch0:Wire, ch1:Wire1
  //Wire.begin();
  //Wire1.begin(32, 33);

  // The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  // ch0:Serial
  // ch1:Serial1
  // ch2:Serial2
  // ch3:SerialBT
  LinxSerialConnection.Start(LinxDevice, 3);
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();

  //Your Code Here, But It will Slow Down The Connection With LabVIEW
  delay(1);
}
