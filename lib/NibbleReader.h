#pragma once
#include <Arduino.h>

class NibbleReader {
public:
    NibbleReader();

    void begin(bool usePullups = false, bool invertLogic = false);

    bool hasChanged();
    uint8_t getLatestNibble() const;
    uint32_t getChangeCount() const;

    // Called by the ISR Wrapper
    void handleInterrupt();
private:
    static NibbleReader* instance;
    static void bindInstance(NibbleReader* inst);

    volatile uint8_t latestNibble_;
    volatile uint8_t previousNibble_;
    volatile bool nibbleChanged_;
    volatile uint32_t changeCount_;

    // stability filter
    volatile uint8_t stableNibble_;
    volatile uint8_t stableCandidate_;
    volatile uint8_t stableCount_;

    bool invertLogic_;

    uint8_t readRawNibble() const;

public:
    // ISR Bridge
    static void onPortBInterrrupt();
};