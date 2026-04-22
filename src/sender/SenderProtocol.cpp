#include "SenderProtocol.h"
#include "../shared/protocol.h"

SenderProtocol::SenderProtocol() {}

void SenderProtocol::begin() {
    // PB0..PB4 outputs
    DDRB |= OWNED_MASK;
    clearOwnedPins();
}

void SenderProtocol::clearOwnedPins() {
    PORTB &= ~OWNED_MASK;
}

void SenderProtocol::writeNibble(uint8_t nibble) {
    nibble &= 0x0F;

    uint8_t portBits = (nibble << 2) & DATA_MASK;

    uint8_t preserved = PORTB & ~OWNED_MASK;
    PORTB = preserved | portBits;
}

void SenderProtocol::pulseCheckpoint() {
    PORTB |= CHECK_MASK;
    delay(Protocol::PULSE_WIDTH_MS);
    PORTB &= ~CHECK_MASK;
    delay(Protocol::POST_PULSE_DELAY_MS);
}

void SenderProtocol::sendChar(char c) {
    uint8_t value = static_cast<uint8_t>(c);

    uint8_t hi = Protocol::highNibble(value);
    uint8_t lo = Protocol::lowNibble(value);

    writeNibble(hi);
    delay(Protocol::NIBBLE_DELAY_MS);
    pulseCheckpoint();

    writeNibble(lo);
    delay(Protocol::NIBBLE_DELAY_MS);
    pulseCheckpoint();
}

void SenderProtocol::sendString(const char* str) {
    while (*str) {
        sendChar(*str++);
    }
}