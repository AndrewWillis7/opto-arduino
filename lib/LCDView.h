#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "../lib/CharReciever.h"

class LcdView {
public:
    LcdView(uint8_t rs, uint8_t e, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

    void begin();
    void show(uint8_t nibble, uint8_t byteVal, char ch, uint32_t count, uint8_t nibbleCount);

private:
    LiquidCrystal lcd_;

    void printNibbleBinary(uint8_t value);
    void printByteHex(uint8_t value);
};