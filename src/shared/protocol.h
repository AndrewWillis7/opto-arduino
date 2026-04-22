#pragma once
#include <Arduino.h>

namespace Protocol {
    constexpr uint16_t NIBBLE_DELAY_MS   = 100;
    constexpr uint16_t PULSE_WIDTH_MS    = 100;
    constexpr uint16_t POST_PULSE_DELAY_MS = 50;

    struct FramePins {
        uint8_t bit0;
        uint8_t bit1;
        uint8_t bit2;
        uint8_t bit3;
        uint8_t checkpoint;
    };

    inline uint8_t makeByte(uint8_t hi, uint8_t lo) {
        return ((hi & 0x0F) << 4) | (lo & 0x0F);
    }

    inline uint8_t highNibble(uint8_t value) {
        return (value >> 4) & 0x0F;
    }

    inline uint8_t lowNibble(uint8_t value) {
        return value & 0x0F;
    }
}