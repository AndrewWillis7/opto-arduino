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
};

class CharReceiver {
public:
    CharReceiver();

    void update(const PortBSnapshot& portSnap);
    CharSnapshot getSnapshot();

private:
    uint8_t firstNibble_;
    uint8_t secondNibble_;
    uint8_t nibbleCount_;

    uint8_t assembledByte_;
    bool byteReady_;
    char latestChar_;
    uint32_t completedCount_;
};