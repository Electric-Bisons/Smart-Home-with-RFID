#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <string.h>
#include "FS.h"

// Replace with your network credentials
const char *ssid = "banana";
const char *password = "12356789";
const char *host = "192.168.11.4";

//store the uid
String u = "";
String getUID();

void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    //wait for the connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    //show the ssid and IP
    Serial.println("Connected to ");
    Serial.println(ssid);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    u = getUID();

    if (u != "")
    {
        WiFiClient client;

        if (client.connect(host, 100))
        {
            Serial.println("Client connected");
        }

        if (client.connected())
        {
            client.println(u);
        }

        delay(500);
        client.stop();
        Serial.println("Client disconnected");
        u = "";
    }
}
String getUID()
{
    String UID = "";
    char inChar = 0;
    char inData[8] = "";
    int index1 = 0;

    // Read from Rx from atmega16
    if (Serial.available() > 0)
    {
        // read till 8th character
        while (Serial.available() > 0 && index1 < 8)
        {
            delay(100);
            // start reading serially and save to variable
            inChar = Serial.read();
            inData[index1] = inChar + 8;
            index1++;
            // Add a null at the end
            inData[index1] = '\0';
        }

        UID.toUpperCase();
        
        for (byte i = 0; i < 8; i++)
        {
            // concat strings
            UID.concat(String(inData[i]));
        }

        return UID;
    }
}
