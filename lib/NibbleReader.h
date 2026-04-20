#pragma once
#include <Arduino.h>

struct PortBSnapshot {
    uint8_t nibble;         // PB0..PB3
    bool checkpoint;        // PB4
    bool nibbleChanged;
    bool checkpointRising;
    uint32_t eventCount;
};

class NibbleReader {
public:
    NibbleReader();

    void begin(bool usePullups = false, bool invertNibble = false, bool invertCheckpoint = false);
    PortBSnapshot getSnapshot();

    void handleInterrupt();
    static void onPortBInterrupt();

private:
    static NibbleReader* instance_;
    static void bindInstance(NibbleReader* inst);

    volatile uint8_t latestNibble_;
    volatile bool checkpoint_;
    volatile bool nibbleChanged_;
    volatile bool checkpointRising_;
    volatile uint32_t eventCount_;

    bool invertNibble_;
    bool invertCheckpoint_;

    uint8_t readNibbleRaw() const;
    bool readCheckpointRaw() const;
};