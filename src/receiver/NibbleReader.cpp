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

/*
PB5 PB4 PB3 PB2
13  12  11  10

Need nibble result:
bit3 bit2 bit1 bit0
*/

uint8_t NibbleReader::readNibbleRaw() const {
    uint8_t raw = PINB;

    uint8_t nibble =
        ((raw >> 2) & 0x01) |   // PB2 -> bit0
        ((raw >> 2) & 0x02) |   // PB3 -> bit1
        ((raw >> 2) & 0x04) |   // PB4 -> bit2
        ((raw >> 2) & 0x08);    // PB5 -> bit3

    if (invertNibble_) {
        nibble = (~nibble) & 0x0F;
    }

    return nibble;
}

bool NibbleReader::readCheckpointRaw() const {
    bool cp = (PINB & (1 << PB1)) != 0;   // D9

    if (invertCheckpoint_) {
        cp = !cp;
    }

    return cp;
}

void NibbleReader::begin(bool usePullups,
                         bool invertNibble,
                         bool invertCheckpoint) {

    invertNibble_ = invertNibble;
    invertCheckpoint_ = invertCheckpoint;

    pinMode(10, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(11, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(12, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(13, usePullups ? INPUT_PULLUP : INPUT);
    pinMode(9,  usePullups ? INPUT_PULLUP : INPUT);

    noInterrupts();

    latestNibble_ = readNibbleRaw();
    checkpoint_ = readCheckpointRaw();
    nibbleChanged_ = true;
    checkpointRising_ = false;
    eventCount_ = 0;

    bindInstance(this);

    PCICR |= (1 << PCIE0);

    // PB1..PB5 = D9..D13
    PCMSK0 |= (1 << PCINT1);
    PCMSK0 |= (1 << PCINT2);
    PCMSK0 |= (1 << PCINT3);
    PCMSK0 |= (1 << PCINT4);
    PCMSK0 |= (1 << PCINT5);

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