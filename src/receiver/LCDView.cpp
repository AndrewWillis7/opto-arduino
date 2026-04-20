#include "../lib/LCDView.h"

LcdView::LcdView(uint8_t rs, uint8_t e, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    : lcd_(rs, e, d4, d5, d6, d7) {}

void LcdView::begin() {
    lcd_.begin(16, 2);
    lcd_.clear();

    lcd_.setCursor(0, 0);
    lcd_.print("N:");
    lcd_.setCursor(8, 0);
    lcd_.print("Cnt:");

    lcd_.setCursor(0, 1);
    lcd_.print("C:");
}

void LcdView::printNibbleBinary(uint8_t value) {
    for (int i = 3; i >= 0; --i) {
        lcd_.print((value >> i) & 0x01);
    }
}

void LcdView::printByteHex(uint8_t value) {
    const char hex[] = "0123456789ABCDEF";
    lcd_.print(hex[(value >> 4) & 0x0F]);
    lcd_.print(hex[value & 0x0F]);
}

void LcdView::show(uint8_t nibble, uint8_t byteVal, char ch, uint32_t count, uint8_t nibbleCount) {
    lcd_.setCursor(2, 0);
    printNibbleBinary(nibble);
    lcd_.print(" ");
    lcd_.print(nibbleCount);

    lcd_.setCursor(12, 0);
    lcd_.print("    ");
    lcd_.setCursor(12, 0);
    lcd_.print(count);

    lcd_.setCursor(2, 1);
    lcd_.print("0x");
    printByteHex(byteVal);
    lcd_.print(" ");

    if (ch >= 32 && ch <= 126) {
        lcd_.print(ch);
    } else {
        lcd_.print(".");
    }

    lcd_.print("   ");
}