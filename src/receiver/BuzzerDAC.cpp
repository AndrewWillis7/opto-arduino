#include "../lib/BuzzerDAC.h"

BuzzerDAC::BuzzerDAC()
    :   ready_(false) {}

bool BuzzerDAC::begin(uint8_t i2cAddress) {
    Wire.begin();
    ready_ = dac_.begin(i2cAddress);
    return ready_;
}

void BuzzerDAC::beep(uint16_t frequencyHz, uint16_t durationMs) {
    if (!ready_ || frequencyHz == 0 || durationMs == 0) return;

    // square wave using a full scale swing
    const uint16_t highVal = 4095;
    const uint16_t lowVal = 0;

    const unsigned long periodUs = 1000000UL / frequencyHz;
    const unsigned long halfPeriodUs = periodUs / 2;
    const unsigned long totalCycles = 
        ((unsigned long)frequencyHz * (unsigned long)durationMs) / 1000UL;
    
    for (unsigned long i = 0; i < totalCycles; ++i) {
        dac_.setVoltage(highVal, false);
        delayMicroseconds(halfPeriodUs);

        dac_.setVoltage(lowVal, false);
        delayMicroseconds(halfPeriodUs);
    }

    dac_.setVoltage(0, false);
}