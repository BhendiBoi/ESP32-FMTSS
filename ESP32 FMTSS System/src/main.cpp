#include <Arduino.h>
// #include <Adafruit_MLX90614.h> //Remember to Modify!! (Comment "Wire.begin()" in Adafruit_SSD1306.CPP)
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <WebServerFunctions.h> //Library containing all necessary camera pins and server initialization functions

int streamPort = 80;
int IOPort = 81;

WSF streamserver(streamPort); 
WSF IOserver(IOPort); 

//Replace with your network credentials
const char *ssid = "JioFiber -  2.4";  //uncomment (made to remind replacement of SSID and Password)
const char *password = "Jio@1102";

// Set your Static IP address
IPAddress local_IP(192, 168, 29, 199);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);

WiFiServer server(IOPort);
// String header;

// // Current time
// unsigned long currentTime = millis();
// // Previous time
// unsigned long previousTime = 0;
// // Define timeout time in milliseconds (example: 2000ms = 2s)
// const long timeoutTime = 2000;

//Adafruit_MLX90614 MLX = Adafruit_MLX90614();

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  wifiConnect(ssid, password, local_IP, gateway, subnet);
  streamserver.startCameraServer();
  IOserver.startIOServer(server);
  // Wire.begin(13, 15);
  // MLX.begin();
  // display.begin(SSD1306_SWITCHCAPVCC);
  // display.display();
  // display.clearDisplay();
}
void loop()
{
  // // put your main code here, to run repeatedly:
  // WiFiClient client = server.available(); // Listen for incoming clients

  // if (client)
  // { // If a new client connects,
  //   currentTime = millis();
  //   previousTime = currentTime;
  //   Serial.println("New Client."); // print a message out in the serial port
  //   String currentLine = "";       // make a String to hold incoming data from the client
  //   while (client.connected() && currentTime - previousTime <= timeoutTime)
  //   { // loop while the client's connected
  //     currentTime = millis();
  //     if (client.available())
  //     {                         // if there's bytes to read from the client,
  //       char c = client.read(); // read a byte, then
  //       Serial.write(c);        // print it out the serial monitor
  //       header += c;
  //       if (c == '\n')
  //       { // if the byte is a newline character
  //         // if the current line is blank, you got two newline characters in a row.
  //         // that's the end of the client HTTP request, so send a response:
  //         if (currentLine.length() == 0)
  //         {
  //           // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  //           // and a content-type so the client knows what's coming, then a blank line:
  //           client.println("HTTP/1.1 200 OK");
  //           client.println("Content-type:text/html");
  //           client.println("Connection: close");
  //           client.println();

  //           // turns the GPIOs on and off
  //           if (header.indexOf("GET /mask") >= 0)
  //           {
  //             Serial.println("Mask Detected");
  //           }
  //           else if (header.indexOf("GET /nomask") >= 0)
  //           {
  //             Serial.println("Mask Not Detected");
  //           }

  //           // Display the HTML web page
  //           client.println("<!DOCTYPE html><html>");
  //           client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  //           client.println("<link rel=\"icon\" href=\"data:,\">");
  //           // CSS to style the on/off buttons
  //           // Feel free to change the background-color and font-size attributes to fit your preferences
  //           client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  //           client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  //           client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  //           client.println(".button2 {background-color: #555555;}</style></head>");

  //           // Web Page Heading
  //           client.println("<body><h1>ESP32 Web Server</h1>");

  //           client.println("</body></html>");

  //           // The HTTP response ends with another blank line
  //           client.println();
  //           // Break out of the while loop
  //         }
  //         else
  //         { // if you got a newline, then clear currentLine
  //           currentLine = "";
  //         }
  //       }
  //       else if (c != '\r')
  //       {                   // if you got anything else but a carriage return character,
  //         currentLine += c; // add it to the end of the currentLine
  //       }
  //     }
  //   }
  // }
}
