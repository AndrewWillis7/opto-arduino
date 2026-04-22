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

    // Map nibble directly into PB0..PB3
    uint8_t preserved = PORTB & ~OWNED_MASK;
    PORTB = preserved | nibble;
}

void SenderProtocol::pulseCheckpoint() {
    // IMPORTANT:
    // Keep the current nibble stable while pulsing checkpoint.
    // Do NOT zero data lines here, or the receiver may interpret
    // that transition as a new nibble.
    PORTB |= CHECK_MASK;
    delay(Protocol::PULSE_WIDTH_MS);
    PORTB &= ~CHECK_MASK;
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

    // Optional cleanup delay so next byte starts from a clean state
    delay(Protocol::POST_CHAR_DELAY_MS);
}

void SenderProtocol::sendString(const char* str) {
    while (*str) {
        sendChar(*str++);
    }
}