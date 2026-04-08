#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>

class LCDView {
public:
    LCDView(uint8_t rs, uint8_t e, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

    void begin();
    void showNibble(uint8_t nibble, uint32_t changeCount);

private:
    LiquidCrystal lcd_;
    uint8_t lastNibble_;
    uint32_t lastCount_;

    void printNibbleBinary(uint8_t value);
};