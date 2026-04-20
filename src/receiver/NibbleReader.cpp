#include "../lib/NibbleReader.h"

NibbleReader* NibbleReader::instance_ = nullptr;

NibbleReader::NibbleReader()
    : latestNibble_(0),
      checkpoint_(false),
      nibbleChanged_(false),
      checkpointRising_(false),
      eventCount_(0),
      invertNibble_(false),
      invertCheckpoint_(false) {}

void NibbleReader::bindInstance(NibbleReader* inst) {
    instance_ = inst;
}

uint8_t NibbleReader::readNibbleRaw() const {
    uint8_t nibble = PINB & 0x0F;     // D8-D11
    if (invertNibble_) {
        nibble = (~nibble) & 0x0F;
    }
    return nibble;
}

bool NibbleReader::readCheckpointRaw() const {
    bool cp = (PINB & (1 << PB4)) != 0;   // D12
    if (invertCheckpoint_) {
        cp = !cp;
    }
    return cp;
}

void NibbleReader::begin(bool usePullups, bool invertNibble, bool invertCheckpoint) {
    invertNibble_ = invertNibble;
    invertCheckpoint_ = invertCheckpoint;

    pinMode(8,  usePullups ? INPUT_PULLUP : INPUT);
    pinMode(9,  usePullups ? INPUT_PULLUP : INPUT);
    pinMode(10, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(11, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(12, usePullups ? INPUT_PULLUP : INPUT);

    noInterrupts();

    latestNibble_ = readNibbleRaw();
    checkpoint_ = readCheckpointRaw();
    nibbleChanged_ = true;
    checkpointRising_ = false;
    eventCount_ = 0;

    bindInstance(this);

    PCICR |= (1 << PCIE0);

    // PB0..PB4 = D8..D12
    PCMSK0 |= (1 << PCINT0);
    PCMSK0 |= (1 << PCINT1);
    PCMSK0 |= (1 << PCINT2);
    PCMSK0 |= (1 << PCINT3);
    PCMSK0 |= (1 << PCINT4);

    interrupts();
}

void NibbleReader::handleInterrupt() {
    uint8_t newNibble = readNibbleRaw();
    bool newCheckpoint = readCheckpointRaw();

    if (newNibble != latestNibble_) {
        latestNibble_ = newNibble;
        nibbleChanged_ = true;
        eventCount_++;
    }

    if (!checkpoint_ && newCheckpoint) {
        checkpointRising_ = true;
        eventCount_++;
    }

    checkpoint_ = newCheckpoint;
}

PortBSnapshot NibbleReader::getSnapshot() {
    noInterrupts();
    PortBSnapshot snap;
    snap.nibble = latestNibble_;
    snap.checkpoint = checkpoint_;
    snap.nibbleChanged = nibbleChanged_;
    snap.checkpointRising = checkpointRising_;
    snap.eventCount = eventCount_;

    nibbleChanged_ = false;
    checkpointRising_ = false;
    interrupts();

    return snap;
}

void NibbleReader::onPortBInterrupt() {
    if (instance_) {
        instance_->handleInterrupt();
    }
}

ISR(PCINT0_vect) {
    NibbleReader::onPortBInterrupt();
}