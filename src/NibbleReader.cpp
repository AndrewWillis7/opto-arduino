#include "../lib/NibbleReader.h"
#include "../lib/Configuration.h"

NibbleReader* NibbleReader::instance = nullptr;

NibbleReader::NibbleReader()
    :   latestNibble_(0),
        previousNibble_(0),
        nibbleChanged_(false),
        changeCount_(0),
        stableNibble_(0),
        stableCandidate_(0),
        stableCount_(0),
        invertLogic_(false) {}
    
void NibbleReader::bindInstance(NibbleReader* inst) {
    instance = inst;
}

void NibbleReader::begin(bool usePullups, bool invertLogic) {
    invertLogic_ = invertLogic;

    pinMode(8, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(9, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(10, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(11, usePullups ? INPUT_PULLUP : INPUT);

    uint8_t initialNibble = readRawNibble();

    noInterrupts();

    latestNibble_ = initialNibble;
    previousNibble_ = initialNibble;
    stableNibble_ = initialNibble;
    stableCandidate_ = initialNibble;
    stableCount_ = 2;
    nibbleChanged_ = true;
    changeCount_ = 0;

    bindInstance(this);

    // Enable pin change interrupt group
    PCICR |= (1 << PCIE0);

    // Enable PB0..PB3 = D8..D11
    PCMSK0 |= (1 << PCINT0);
    PCMSK0 |= (1 << PCINT1);
    PCMSK0 |= (1 << PCINT2);
    PCMSK0 |= (1 << PCINT3);

    interrupts();
}

void NibbleReader::handleInterrupt() {
    uint8_t rawNibble = readRawNibble();

    if (rawNibble == stableCandidate_) {
        if (stableCandidate_ < 255) {
            stableCount_++;
        }
    } else {
        stableCandidate_ = rawNibble;
        stableCount_ = 1;
    }

    if (stableCount_ >= 2) {
        stableNibble_ = stableCandidate_;

        if (stableNibble_ != previousNibble_) {
            previousNibble_ = stableNibble_;
            latestNibble_ = stableNibble_;
            nibbleChanged_ = true;
            changeCount_++;
        }
    }
}

bool NibbleReader::hasChanged() {
    noInterrupts();
    bool changed = nibbleChanged_;
    nibbleChanged_ = false;
    interrupts();
    return changed;
}

uint8_t NibbleReader::getLatestNibble() const {
    noInterrupts();
    uint8_t value = latestNibble_;
    interrupts();
    return value;
}

uint32_t NibbleReader::getChangeCount() const {
    noInterrupts();
    uint32_t value = changeCount_;
    interrupts();
    return value;
}

void NibbleReader::onPortBInterrrupt() {
    if (instance) {
        instance->handleInterrupt();
    }
}

ISR(PCINT0_vect) {
    NibbleReader::onPortBInterrrupt();
}
