#include <Arduino.h>
#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]
#define ACTIVATED LOW

class DFF
{
public:
    DFF(int RX, int TX);
    void begin();
    void setVolume(int volume);
    void play(int file);

private:
    int _RX;
    int _TX;
    void execute_CMD(byte CMD, byte Par1, byte Par2);
}