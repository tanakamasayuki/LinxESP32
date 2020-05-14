/****************************************************************************************
**  LINX ESP32 Wifi listener code.
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**
**  Written By Ken Sharp
**
** BSD2 License.
****************************************************************************************/

/****************************************************************************************
**  Includes
****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WiFi.h>

#include "utility/LinxDevice.h"
#include "utility/LinxListener.h"
#include "LinxESP32WifiListener.h"

/****************************************************************************************
**  Constructors
****************************************************************************************/
LinxESP32WifiListener::LinxESP32WifiListener()
{
  State = START;

  wifiState = sINIT;
  LinxWifiConnectStatus = WL_IDLE_STATUS;
  LinxWifiTimeout = 2000;
  //unsigned char wifiServerFail = 0;

}

/****************************************************************************************
**  Functions
****************************************************************************************/
int LinxESP32WifiListener::SetSsid(const char ssid[])
{
  //Copy User SSID Into Member Var
  for (int  i = 0; i < 32; i++)
  {
    LinxWifiSsid[i] = ssid[i];
  }
  return L_OK;
}

int LinxESP32WifiListener::SetSecurity(SecurityType securityType)
{
  LinxWifiSecurity = securityType;
  return L_OK;
}

int LinxESP32WifiListener::SetPassphrase(const char pw[])
{
  //Copy User PW Into Member Var
  for (int  i = 0; i < 32; i++)
  {
    LinxWifiPw[i] = pw[i];
  }
  return L_OK;
}

int LinxESP32WifiListener::Start(LinxDevice* linxDev, unsigned short port)
{
  LinxDev = linxDev;

  recBuffer = (unsigned char*) malloc(LinxDev->ListenerBufferSize);
  sendBuffer = (unsigned char*) malloc(LinxDev->ListenerBufferSize);

  LinxDev->DebugPrintln("Network Wifi Stack :: Starting");

  LinxWifiPort = port;

  State = START;

  LinxDev->DebugPrint("Connecting To Wifi Network");
  return L_OK;

}

int LinxESP32WifiListener::PrintWifiInfo()
{
  //SSID
  const char * szSsid;
  szSsid = LinxWifiSsid;

  //PW
  const char * szPassPhrase;
  szPassPhrase = LinxWifiPw;

  //LINX WIFI DEBUG INFO
  LinxDev->DebugPrintln("");
  LinxDev->DebugPrintln("");
  LinxDev->DebugPrintln(".: LINX WIFI SETTINNGS :.");

  LinxDev->DebugPrint("IP Address : ");
  IPAddress deviceIpAddress = WiFi.localIP();
  LinxDev->DebugPrint(deviceIpAddress[0], DEC);
  LinxDev->DebugPrint(".");
  LinxDev->DebugPrint(deviceIpAddress[1], DEC);
  LinxDev->DebugPrint(".");
  LinxDev->DebugPrint(deviceIpAddress[2], DEC);
  LinxDev->DebugPrint(".");
  LinxDev->DebugPrintln(deviceIpAddress[3], DEC);

  LinxDev->DebugPrint("Port       : ");
  LinxDev->DebugPrintln(LinxWifiPort);

  LinxDev->DebugPrint("SSID       : ");
  LinxDev->DebugPrintln(szSsid);

  LinxDev->DebugPrint("Security   : ");
  switch (LinxWifiSecurity)
  {
    case NONE:
      //No Security
      LinxDev->DebugPrintln("None");
      break;
    case WPA2_PASSPHRASE:
      //WPA2 Passphrase
      LinxDev->DebugPrintln("WPA2 Passphrase");
      break;
    case WPA2_KEY:
      //WPA2 Key  --Untested--
      LinxDev->DebugPrintln("WPA2 Key - Not Implemented");
      break;
    case WEP40:
      LinxDev->DebugPrintln("WEP40 - Not Implemented");
      break;
    case WEO104:
      LinxDev->DebugPrintln("WEP104 - Not Implemented");
      break;
    default:
      break;
  }

  LinxDev->DebugPrint("Passphrase : ");
  LinxDev->DebugPrintln(szPassPhrase);

  LinxDev->DebugPrintln("");
  LinxDev->DebugPrintln("");

  return L_OK;
}



int LinxESP32WifiListener::Init()
{
  //This case will be called repeatedly until the device connects to the network and starts listening.

  //SSID
  const char * szSsid;
  szSsid = LinxWifiSsid;

  //PW
  const char * szPassPhrase;
  szPassPhrase = LinxWifiPw;

  //char key[64];

  //for (int i = 0; i < 64; i++)
  //{
  //  key[i] = LinxWifiPw[i];
  //}

  switch (LinxWifiSecurity)
  {
    case NONE:
      //No Security
      if (szSsid[0] == '\0') {
        LinxWifiConnectStatus = WiFi.begin();
      } else {
        LinxWifiConnectStatus = WiFi.begin(szSsid);
      }
      break;
    case WPA2_PASSPHRASE:
      //WPA2 Passphrase
      LinxWifiConnectStatus = WiFi.begin(szSsid, szPassPhrase);
      break;
    case WPA2_KEY:
      //WPA2 Key  --Untested--
      //LinxWifiConnectStatus = deIPcK.wfConnect(szSsid, szPassPhrase, &LinxTcpStatus);
      break;
    case WEP40:
      //TODO
      break;
    case WEO104:
      //TODO
      break;
    default:
      break;
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Connect To Network
  if (LinxWifiConnectStatus == WL_CONNECTED)
  {
    LinxDev->DebugPrintln("");
    LinxDev->DebugPrintln("Connected To Wifi Network");
    m_pWifiSvr = new WiFiServer(LinxWifiPort);

    //Start the server
    m_pWifiSvr->begin();
    State = AVAILABLE;
  }

  PrintWifiInfo();
  return L_OK;
}


int LinxESP32WifiListener::Listen()
{
  State = AVAILABLE;
  return L_OK;
}

int LinxESP32WifiListener::Available()
{
  m_WifiClient = m_pWifiSvr->available();
  if (m_WifiClient)
  {
    LinxDev->DebugPrintln("Available Client");
    State = ACCEPT;
  }
  return L_OK;
}

int LinxESP32WifiListener::Accept()
{

  if (m_WifiClient.connected())
  {
    LinxDev->DebugPrintln("Client Connected");
    State = CONNECTED;
    LinxWifiStartTime = (unsigned)millis();
  }
  else
  {
    State = CLOSE;
  }

  return L_OK;
}

int LinxESP32WifiListener::Connected()
{
  //Read Wifi TCP Bytes

  //If There Are Bytes Available Have A Look, If Not Loop (Remain In Read Unless Timeout)
  if (m_WifiClient.available() > 0)
  {
    //Read First Byte, Check If It Is SoF (0xFF)
    if ( (recBuffer[0] = m_WifiClient.read()) == 0xFF)
    {
      //LinxDev->DebugPrintln("Network Stack :: SoF Received");

      //SoF Received, Reset LinxWifiStartTime
      LinxWifiStartTime = (unsigned)millis();

      //SoF Received Wait For Packet Size
      while (m_WifiClient.available() < 1)
      {
        if ( ((unsigned)millis() - LinxWifiStartTime) > LinxWifiTimeout)
        {
          State = CLOSE;
          LinxDev->DebugPrintln("Network Stack :: Rx Timeout (0)");
          break;
        }
      }

      recBuffer[1] = m_WifiClient.read();

      //LinxDev->DebugPrint("Network Stack :: Packet Size Received : ");
      //LinxDev->DebugPrintln(recBuffer[1], HEX);

      //Wait For Rest Of Packet
      while (m_WifiClient.available() < recBuffer[1] - 2)
      {
        if ( ((unsigned)millis() - LinxWifiStartTime) > LinxWifiTimeout)
        {
          State = CLOSE;
          LinxDev->DebugPrintln("Network Stack :: Rx Timeout (1)");
          break;
        }
      }

      //Read Packet
      m_WifiClient.read(&recBuffer[2],  recBuffer[1] - 2);


      //DEBUG PRINT PACKET
      LinxDev->DebugPrint("RX <= ");
      for (int i = 0; i < recBuffer[1]; i++)
      {
        LinxDev->DebugPrint("[");
        LinxDev->DebugPrint(recBuffer[i], HEX);
        LinxDev->DebugPrint("] ");
      }
      LinxDev->DebugPrintln("");


      //Checksum
      if (ChecksumPassed(recBuffer))
      {
        //LinxDev->DebugPrintln("Network Stack :: Packet Checksum Passed");

        //Process Command And Respond
        LinxStatus m_status = (LinxStatus)ProcessCommand(recBuffer, sendBuffer);
        if (m_status == L_DISCONNECT)
        {
          State = CLOSE;
        }
        m_WifiClient.write((const uint8_t *)sendBuffer, (size_t)sendBuffer[1]);
      }
      else
      {
        LinxDev->DebugPrintln("Network Stack :: Checksum Failed");
      }
    }
    else
    {
      State = CLOSE;
      LinxDev->DebugPrintln("Network Stack :: SoF Failed");
    }

    //Data Received, Reset Timeout
    LinxWifiStartTime = (unsigned)millis();
  }
  else if ( ((unsigned)millis() - LinxWifiStartTime) > LinxWifiTimeout)
  {
    //Time Out
    LinxDev->DebugPrintln("Network Stack :: Wifi Timeout");
    LinxDev->DebugPrintln(LinxWifiTimeout, DEC);
    State = CLOSE;

  }

  return L_OK;
}

int LinxESP32WifiListener::Close()
{

  //Close TCP Connection, Return To Listening State
  LinxDev->DebugPrintln("Closing Wifi TCP Connection...");
  if (m_WifiClient.connected())
    m_WifiClient.stop();

  //Assume Wifi Stack Is Ok, But Check For Errors
  State = LISTENING;

  return L_OK;
}

int LinxESP32WifiListener::Exit()
{
  //Something Went Wrong.  Try To Close Connection And Quit.
  m_WifiClient.stop();
  delete m_pWifiSvr;
  State = START;

  LinxDev->DebugPrintln("Restarting Wifi Server...");

  return L_OK;
}

int LinxESP32WifiListener::CheckForCommands()
{
  switch (State)
  {
    case START:
      Init();
      //LinxDev->DebugPrintln("..........START..........");
      break;
    case LISTENING:
      Listen();
      //LinxDev->DebugPrintln("..........LISTENING..........");
      break;
    case AVAILABLE:
      Available();
      //LinxDev->DebugPrintln("..........AVAILABLE..........");
      break;
    case ACCEPT:
      Accept();
      //LinxDev->DebugPrintln("..........ACCEPT..........");
      break;
    case CONNECTED:
      Connected();
      //LinxDev->DebugPrintln("..........CONNECTED..........");
      break;
    case CLOSE:
      Close();
      //LinxDev->DebugPrintln("..........CLOSE..........");
      break;
    case EXIT:
      Exit();
      break;
  }

  //Every Iteration Run Periodic Network Tasks
  delay(1);

  return L_OK;
}

// Pre Instantiate Object
LinxESP32WifiListener LinxWifiConnection;
