#include "WebServerFunctions.h"
#include "Arduino.h"

WSF::WSF(int port)
{
    _port = port;
}

void wifiConnect(const char *ssid, const char *password, IPAddress local_ip, IPAddress gateway, IPAddress subnet)
{
    // Wi-Fi connection
    if (!WiFi.config(local_ip, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected with IP address: http://");
    Serial.println(local_ip);
}

void WSF::startCameraServer()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    camera_config_t camconfig;
    camconfig.ledc_channel = LEDC_CHANNEL_0;
    camconfig.ledc_timer = LEDC_TIMER_0;
    camconfig.pin_d0 = Y2_GPIO_NUM;
    camconfig.pin_d1 = Y3_GPIO_NUM;
    camconfig.pin_d2 = Y4_GPIO_NUM;
    camconfig.pin_d3 = Y5_GPIO_NUM;
    camconfig.pin_d4 = Y6_GPIO_NUM;
    camconfig.pin_d5 = Y7_GPIO_NUM;
    camconfig.pin_d6 = Y8_GPIO_NUM;
    camconfig.pin_d7 = Y9_GPIO_NUM;
    camconfig.pin_xclk = XCLK_GPIO_NUM;
    camconfig.pin_pclk = PCLK_GPIO_NUM;
    camconfig.pin_vsync = VSYNC_GPIO_NUM;
    camconfig.pin_href = HREF_GPIO_NUM;
    camconfig.pin_sscb_sda = SIOD_GPIO_NUM;
    camconfig.pin_sscb_scl = SIOC_GPIO_NUM;
    camconfig.pin_pwdn = PWDN_GPIO_NUM;
    camconfig.pin_reset = RESET_GPIO_NUM;
    camconfig.xclk_freq_hz = 20000000;
    camconfig.pixel_format = PIXFORMAT_JPEG;

    if (psramFound())
    {
        camconfig.frame_size = FRAMESIZE_UXGA;
        camconfig.jpeg_quality = 10;
        camconfig.fb_count = 2;
    }
    else
    {
        camconfig.frame_size = FRAMESIZE_SVGA;
        camconfig.jpeg_quality = 12;
        camconfig.fb_count = 1;
    }

    // Camera init
    esp_err_t err = esp_camera_init(&camconfig);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
    httpd_config_t httpdconfig = HTTPD_DEFAULT_CONFIG();
    httpdconfig.server_port = _port;

    httpd_uri_t index_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL};

    //Serial.printf("Starting web server on port: '%d'\n", httpdconfig.server_port);
    if (httpd_start(&stream_httpd, &httpdconfig) == ESP_OK)
    {
        httpd_register_uri_handler(stream_httpd, &index_uri);
    }
}

void WSF::startIOServer(WiFiServer server)
{
    _server = server;
    _server.begin();
}
void WSF::IOListen()
{
    // put your main code here, to run repeatedly:
    WiFiClient client = _server.available(); // Listen for incoming clients

    if (client)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        while (client.connected() && currentTime - previousTime <= timeoutTime)
        { // loop while the client's connected
            currentTime = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // turns the GPIOs on and off
                        if (header.indexOf("GET /mask") >= 0)
                        {
                            Serial.println("Mask Detected");
                        }
                        else if (header.indexOf("GET /nomask") >= 0)
                        {
                            Serial.println("Mask Not Detected");
                        }

                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #555555;}</style></head>");

                        // Web Page Heading
                        client.println("<body><h1>ESP32 Web Server</h1>");

                        client.println("</body></html>");

                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
    }
}

