#include "SenderProtocol.h"
#include "../shared/protocol.h"

SenderProtocol::SenderProtocol() {}

void SenderProtocol::begin() {
    // PB1..PB5 outputs
    DDRB |= OWNED_MASK;
    clearOwnedPins();
}

void SenderProtocol::clearOwnedPins() {
    PORTB &= ~OWNED_MASK;
}

void SenderProtocol::writeNibble(uint8_t nibble) {
    nibble &= 0x0F;

    // Map nibble bits into PB1..PB4
    // nibble bit0 -> PB1
    // nibble bit1 -> PB2
    // nibble bit2 -> PB3
    // nibble bit3 -> PB4
    uint8_t portBits = (nibble << 1) & DATA_MASK;

    uint8_t preserved = PORTB & ~OWNED_MASK;
    PORTB = preserved | portBits; // checkpoint remains low here
}

void SenderProtocol::pulseCheckpoint() {
    // Keep data lines low during pulse for a clean delimiter
    uint8_t preserved = PORTB & ~OWNED_MASK;
    PORTB = preserved | CHECK_MASK;
    delay(Protocol::PULSE_WIDTH_MS);

    clearOwnedPins();
}

void SenderProtocol::sendChar(char c) {
    uint8_t value = static_cast<uint8_t>(c);

    uint8_t hi = Protocol::highNibble(value);
    uint8_t lo = Protocol::lowNibble(value);

    writeNibble(hi);
    delay(Protocol::NIBBLE_DELAY_MS);

    writeNibble(lo);
    delay(Protocol::NIBBLE_DELAY_MS);

    pulseCheckpoint();
}

void SenderProtocol::sendString(const char* str) {
    while (*str) {
        sendChar(*str++);
    }
}