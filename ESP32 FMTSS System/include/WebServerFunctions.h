#ifndef WEBSERVERFUNCTIONS_H
#define WEBSERVERFUNCTIONS_H

#include "Arduino.h"
#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h"          //disable brownout problems
#include "soc/rtc_cntl_reg.h" //disable brownout problems
#include "esp_http_server.h"
#include "WiFi.h"

#define PART_BOUNDARY "123456789000000000000987654321"

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

void wifiConnect(const char *ssid, const char *password, IPAddress local_ip, IPAddress gateway, IPAddress subnet);
void checkWifiConnection(const char *ssid, const char *password);

class WSF
{
public:
    WSF(int port);
    void startCameraServer();
    void startIOServer(WiFiServer server);
    byte IOListen();

private:
    int _port;
    WiFiServer _server;
    String header;
    // Current time
    unsigned long currentTime;
    // Previous time
    unsigned long previousTime;
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    const long timeoutTime = 2000;
    httpd_handle_t stream_httpd = NULL;
    static esp_err_t stream_handler(httpd_req_t *req)
    {
        camera_fb_t *fb = NULL;
        esp_err_t res = ESP_OK;
        size_t _jpg_buf_len = 0;
        uint8_t *_jpg_buf = NULL;
        char *part_buf[64];

        res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
        if (res != ESP_OK)
        {
            return res;
        }

        while (true)
        {
            fb = esp_camera_fb_get();
            if (!fb)
            {
                Serial.println("Camera capture failed");
                res = ESP_FAIL;
            }
            else
            {
                if (fb->width > 400)
                {
                    if (fb->format != PIXFORMAT_JPEG)
                    {
                        bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                        esp_camera_fb_return(fb);
                        fb = NULL;
                        if (!jpeg_converted)
                        {
                            Serial.println("JPEG compression failed");
                            res = ESP_FAIL;
                        }
                    }
                    else
                    {
                        _jpg_buf_len = fb->len;
                        _jpg_buf = fb->buf;
                    }
                }
            }
            if (res == ESP_OK)
            {
                size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
                res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
            }
            if (res == ESP_OK)
            {
                res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
            }
            if (res == ESP_OK)
            {
                res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
            }
            if (fb)
            {
                esp_camera_fb_return(fb);
                fb = NULL;
                _jpg_buf = NULL;
            }
            else if (_jpg_buf)
            {
                free(_jpg_buf);
                _jpg_buf = NULL;
            }
            if (res != ESP_OK)
            {
                break;
            }
            //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
        }
        return res;
    }
};

#endif
