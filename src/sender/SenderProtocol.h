#pragma once
#include <Arduino.h>

class SenderProtocol {
public:
    SenderProtocol();

    void begin();
    void sendChar(char c);
    void sendString(const char* str);

private:
    static constexpr uint8_t DATA_MASK  = 0b00011110; // PB1..PB4
    static constexpr uint8_t CHECK_MASK = 0b00100000; // PB5
    static constexpr uint8_t OWNED_MASK = DATA_MASK | CHECK_MASK;

    void writeNibble(uint8_t nibble);
    void pulseCheckpoint();
    void clearOwnedPins();
};