#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

class BuzzerDAC {
public:
    BuzzerDAC();

    bool begin(uint8_t i2cAddres = 0x60);
    void beep(uint16_t frequencyHz = 2000, uint16_t durationMs = 120);

private:
    Adafruit_MCP4725 dac_;
    bool ready_;
};