#include <M5StickC.h>
#include <LinxESP32.h>

LinxESP32* LinxDevice;

void setup()
{
  /*
    note: Change Partition Scheme(Default -> NoOTA or MinimalSPIFFS)
  */
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);

  LinxDevice = new LinxESP32();

  // PWM & Servo Setup
  uint8_t pwmList[] = {};    // Max16Ch {0, 26, ...}
  uint16_t pwmFrequency = 12000;  // LED:12000, Servo:50
  for (int i = 0; i < sizeof(pwmList); i++) {
    ledcSetup(i, pwmFrequency, 8);
    pinMode(pwmList[i], OUTPUT);
    ledcAttachPin(pwmList[i], i);
  }

  // I2C Begin ch0:Wire, ch1:Wire1
  //Wire.begin(0, 26);
  //Wire1.begin(32, 33);

  // The LINX Listener Is Pre Instantiated.
  // Set SSID (Network Name), Security Type, Passphrase/Key, And Call Start With Desired Device IP and Port
  // If not set, it will connect with the last connection information
  //LinxWifiConnection.SetSsid("YOUR_NETWORK_NAME");
  //LinxWifiConnection.SetSecurity(WPA2_NONE);  //NONE, WPA2_PASSPHRASE, WPA2_KEY, WEP40, WEO104
  //LinxWifiConnection.SetPassphrase("PASSPHRASE");

  // Wi-Fi
  LinxWifiConnection.Start(LinxDevice, 44300);

  // The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  // ch0:Serial
  // ch1:Serial1
  // ch2:Serial2
  // ch3:SerialBT
  LinxSerialConnection.Start(LinxDevice, 3);
}

void loop()
{
  LinxWifiConnection.CheckForCommands();
  LinxSerialConnection.CheckForCommands();

  M5.update();
  if (millis() % 1000 == 0) {
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("LinxESP32");
    M5.Lcd.print("IP  : ");
    M5.Lcd.println(WiFi.localIP());
    byte macAddress[6];
    WiFi.macAddress(macAddress);
    M5.Lcd.printf("MAC : %02X:%02X:%02X:%02X:%02X:%02X\n", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
  }

  if (M5.BtnB.wasPressed()) {
    ESP.restart();
  }

  delay(1);
}
