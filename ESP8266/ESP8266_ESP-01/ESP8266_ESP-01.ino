#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <WiFiUdp.h>

//AP config
const char* ssid = "HUAWEI-231D";
const char* password = "23223462";

//SNTP SERVER
unsigned int localPort = 2390;
IPAddress timeServerIP;
const char* ntpServerName = "0.south-america.pool.ntp.org";
const int NTP_PACKET_SIZE = 48;      // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP udp;

//TCP Server
WiFiServer server(8080);
static WiFiClient client;

void setup() 
{
    Serial.begin(115200);
    WiFi.begin(ssid,password);
    while(WiFi.status() != WL_CONNECTED)
      delay(500);
    
    server.begin();

    udp.begin(localPort);
}

void loop() 
{
    client = server.available();
    if (client)
    {
        client.setTimeout(200); 
        client.print("timeout");
  
        while (client.connected())
        {
            String line = client.readStringUntil('\r');
            if(line != "timeout")
                checkClientRcvData(line);
            checkSerialRcvData();
        }
    }
}

unsigned long sendNTPpacket(IPAddress& address)
{
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;            // Stratum, or type of clock
    packetBuffer[2] = 6;            // Polling Interval
    packetBuffer[3] = 0xEC;         // Peer Clock Precision
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    udp.beginPacket(address, 123);  //NTP requests are to port 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
}

unsigned long getUnixTime(void)
{    
    WiFi.hostByName(ntpServerName, timeServerIP); 
    sendNTPpacket(timeServerIP);
    delay(500);          
    int cb = udp.parsePacket();
    if (!cb) 
        return 0;
    else 
    {
        udp.read(packetBuffer, NTP_PACKET_SIZE);
    
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        unsigned long secsSince1900 = highWord << 16 | lowWord;
    
        const unsigned long seventyYears = 2208988800UL;
        unsigned long epoch = secsSince1900 - seventyYears;
        return epoch;
    }
}

void checkClientRcvData(String line)
{
    if(line.length() > 3)
    {
        if(line.substring(0,3) == "$$$")
        {
            //configurarRTC ($$$unixtime'\n')
            if(line.substring(3,11) == "unixtime")
            {
                unsigned long unixTime = getUnixTime();
                if(unixTime != 0)
                {
                    String dato = String(unixTime);
                    Serial.println("$$$u" + dato + "\n");
                }
            }
            //configurarTemperatura ($$$t(largo)(temperatura)'\n')
            if(line.substring(3,4) == "t")
            {
                int largoTemp = line.substring(4,5).toInt();
                int temp = line.substring(5, (5 + largoTemp)).toInt();
                if(temp >= 0 && temp <= 100)
                {
                    Serial.println("$$$t" + String(largoTemp) + String(temp) + "\n");
                }
            }
        }
    }
    else
      client.print("timeout");
}

void checkSerialRcvData(void)
{    
    if(Serial.available() > 0)
    {      
        String serialData = "";
        while(Serial.available() > 0) // Don't read unless
        {
            char inChar = Serial.read(); // Read a character
            serialData += inChar; // Store it
            if(inChar == '\n' || inChar == '\0')
            {
                serialData += '\0';
            }
        }
        
        if(serialData.length() > 16)
        {                
            if(serialData.substring(0,6) == "$$$log") //log Temperatura real time
            {
                unsigned long unixTime = serialData.substring(6,16).toInt();
                int largoTemp = serialData.substring(16,17).toInt();
                int temp = serialData.substring(17, (17 + largoTemp)).toInt();
                if(unixTime > 1044043320 && temp > -200 && temp < 850)
                {
                    client.print("$$$log" + String(unixTime) + String(largoTemp) + String(temp) + "\n");
                }
            }
        }
        if(serialData.length() > 9)
        {
            if(serialData.substring(0,9) == "$$$status") //status calentador
            {
                int estadoCalentador = serialData.substring(9,10).toInt();
                if(estadoCalentador == 1 || estadoCalentador == 0)
                {
                    client.print("$$$sta" + String(estadoCalentador) + "\n");
                }
            }
        }
    }
}
