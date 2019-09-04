#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <string.h>
#include "FS.h"
#define ID_NR 4

// Replace with your network credentials
const char *ssid = "banana";
const char *password = "12356789";
const char *host = "192.168.11.4";

IPAddress ip(192, 168, 11, 4);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 255, 0);

//data declarations
String tags[ID_NR] = {"162BAD58 ", "45ECR66B ", "81770DC5 ", "063F237E "};
File data;
String answer = "";
// Set web server port number to 80
ESP8266WebServer server(80);
WiFiServer server2(100);

void webpage()
{
    server.sendHeader("Location", "/index.html", true); //Redirect to our txt file
    server.send(302, "text/plane", "");
}

void dataHandler()
{
    server.sendHeader("Location", "/test.txt", true); //Redirect to our txt file
    server.send(302, "text/plane", "");
}

void setup()
{
    for (unsigned int i = 0; i < ID_NR; ++i)
    {
        tags[i].setCharAt(tags[i].length() - 1, '\0');
    }

    Serial.begin(9600);
    SPIFFS.begin();

    SPIFFS.remove("/test.txt");
    data = SPIFFS.open("/test.txt", "a+");

    // Rountes of the server
    server.on("/", webpage);
    server.on("/data", dataHandler);
    server.onNotFound(handleWebRequests);

    //set server as AP
    WiFi.mode(WIFI_AP);
    //set server ip, gateway, subnet
    WiFi.softAPConfig(ip, gateway, subnet);

    //create server
    Serial.print("Creating server... ");
    Serial.println(ssid);
    WiFi.softAP(ssid, password);
    //start the server
    server.begin();
    server2.begin();
    Serial.println("SERVER STARTED");
}

void loop()
{
    server.handleClient();
    WiFiClient client = server2.available();

    if (client)
    {
        int flag = 0;

        Serial.println("New client");
        answer = client.readStringUntil('\n');
        answer.setCharAt(answer.length() - 1, '\0');

        for (unsigned int index = 0; index < answer.length() - 1; ++index)
        {
            answer.setCharAt(index, answer.charAt(index) - 8);
        }

        Serial.println(answer);

        if ( (answer.equals(tags[0])) || (answer.equals(tags[1])) )
        {
            data.println(answer + " user Andrei ");
            Serial.write(0xFF);
            flag = 1;
        }
        if ( (answer.equals(tags[2])) || (answer.equals(tags[3])) )
        {
            data.println(answer + " user Gabi ");
            Serial.write(0xF0);
            flag = 1;
        }

        if (flag == 0)
        {
            data.println(answer + " acces denied...RUN! ");
            Serial.write(0x0F);
        }
    }
}

bool loadFromSpiffs(String path)
{
    String dataType = "text/plain";
    if (path.endsWith("/"))
        path += "index.htm";
    if (path.endsWith(".src"))
        path = path.substring(0, path.lastIndexOf("."));
    else if (path.endsWith(".html"))
        dataType = "text/html";
    else if (path.endsWith(".htm"))
        dataType = "text/html";
    else if (path.endsWith(".css"))
        dataType = "text/css";
    else if (path.endsWith(".js"))
        dataType = "application/javascript";
    else if (path.endsWith(".png"))
        dataType = "image/png";
    else if (path.endsWith(".gif"))
        dataType = "image/gif";
    else if (path.endsWith(".jpg"))
        dataType = "image/jpeg";
    else if (path.endsWith(".ico"))
        dataType = "image/x-icon";
    else if (path.endsWith(".xml"))
        dataType = "text/xml";
    else if (path.endsWith(".pdf"))
        dataType = "application/pdf";
    else if (path.endsWith(".zip"))
        dataType = "application/zip";
    File dataFile = SPIFFS.open(path.c_str(), "r");
    if (server.hasArg("download"))
        dataType = "application/octet-stream";
    if (server.streamFile(dataFile, dataType) != dataFile.size())
    {
    }
    dataFile.close();
    return true;
}

//function to handle all web requests
void handleWebRequests()
{
    if (loadFromSpiffs(server.uri()))
        return;
    if (server.uri() == "\value")
    {
        String message = "File Not Detected\n\n";
        message += "URI: ";
        message += server.uri();
        message += "\nMethod: ";
        message += (server.method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server.args();
        message += "\n";
        for (uint8_t i = 0; i < server.args(); i++)
        {
            message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
        }
        server.send(404, "text/plain", message);
        Serial.println(message);
    }
}
