#include "../lib/LCDView.h"
#include <Arduino.h>

LCDView::LCDView(uint8_t rs, uint8_t e, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    :   lcd_(rs, e, d4, d5, d6, d7),
        lastNibble_(0xFF),
        lastCount_(0xFFFFFFFFUL) {}

void LCDView::begin() {
    lcd_.begin(16, 2);
    lcd_.clear();

    lcd_.setCursor(0, 0);
    lcd_.print("Nibble:");

    lcd_.setCursor(0, 1);
    lcd_.print("Dec:    Cnt:");
}

void LCDView::printNibbleBinary(uint8_t value) {
    for (int8_t i = 3; i >= 0; --i) {
        lcd_.print((value >> i) & 0x01);
    }
}

void LCDView::showNibble(uint8_t nibble, uint32_t changeCount) {
    if (nibble != lastNibble_) {
        lcd_.setCursor(7, 0);
        printNibbleBinary(nibble);
        lcd_.print("    ");

        lcd_.setCursor(4, 1);
        if (nibble < 10) lcd_.print(' ');
        lcd_.print(nibble);
        lcd_.print(' ');

        lastNibble_ = nibble;
    }

    if (changeCount != lastCount_) {
        lcd_.setCursor(10, 1);
        lcd_.print("    ");
        lcd_.setCursor(10, 1);
        lcd_.print(changeCount);

        lastCount_ = changeCount;
    }
}