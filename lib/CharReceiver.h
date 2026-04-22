#pragma once
#include <Arduino.h>
#include "../lib/NibbleReader.h"

struct CharSnapshot {
    uint8_t currentNibble;
    uint8_t assembledByte;
    bool byteReady;
    char latestChar;
    uint32_t completedCount;
    uint8_t nibbleCount;
    bool resynced;
};

class CharReceiver {
public:
    CharReceiver();

    void begin(unsigned long nibbleTimeoutMs = 1200);
    void update(const PortBSnapshot& portSnap);
    CharSnapshot getSnapshot();

private:
    void resetAssemblyState();

    uint8_t firstNibble_;
    uint8_t secondNibble_;
    uint8_t nibbleCount_;

    uint8_t assembledByte_;
    bool byteReady_;
    char latestChar_;
    uint32_t completedCount_;

    unsigned long lastCheckpointMs_;
    unsigned long nibbleTimeoutMs_;
    bool resynced_;
};