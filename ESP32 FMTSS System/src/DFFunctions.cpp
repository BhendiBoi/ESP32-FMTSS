#include "DFFunctions.h"
#include "Arduino.h"

DFF::DFF(int RX, int TX)
{
    _RX = RX;
    _TX = TX;
}

void DFF::begin()
{
    Serial2.begin(9600, SERIAL_8N1, _RX, _TX);
    setVolume(25);
}

void DFF::execute_CMD(byte CMD, byte Par1, byte Par2)
{
    // Calculate the checksum (2 bytes)
    word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
    // Build the command line
    byte Command_line[10] = {Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                             Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
    //Send the command line to the module
    for (byte k = 0; k < 10; k++)
    {
        Serial2.write(Command_line[k]);
        //Serial.write(Command_line[k]);
    }
}

void DFF::play(int file)
{
    file = constrain(file, 0, 2999);
    execute_CMD(0x03, 0, file);
}

void DFF::setVolume(int volume)
{
    execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
}
