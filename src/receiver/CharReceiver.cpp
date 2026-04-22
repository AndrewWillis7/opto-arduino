#include "../lib/CharReceiver.h"

CharReceiver::CharReceiver()
    : firstNibble_(0),
      secondNibble_(0),
      nibbleCount_(0),
      assembledByte_(0),
      byteReady_(false),
      latestChar_('?'),
      completedCount_(0),
      lastCheckpointMs_(0),
      nibbleTimeoutMs_(1200),
      resynced_(false) {}

void CharReceiver::begin(unsigned long nibbleTimeoutMs) {
    nibbleTimeoutMs_ = nibbleTimeoutMs;
    resetAssemblyState();
    assembledByte_ = 0;
    byteReady_ = false;
    latestChar_ = '?';
    completedCount_ = 0;
    lastCheckpointMs_ = millis();
    resynced_ = false;
}

void CharReceiver::resetAssemblyState() {
    firstNibble_ = 0;
    secondNibble_ = 0;
    nibbleCount_ = 0;
}

void CharReceiver::update(const PortBSnapshot& portSnap) {
    if (!portSnap.checkpointRising) {
        return;
    }

    unsigned long now = millis();

    // If too much time passed since the previous checkpoint,
    // assume framing was lost and restart nibble assembly.
    if ((now - lastCheckpointMs_) > nibbleTimeoutMs_) {
        resetAssemblyState();
        resynced_ = true;
    }

    lastCheckpointMs_ = now;

    uint8_t nibble = portSnap.nibble & 0x0F;

    if (nibbleCount_ == 0) {
        firstNibble_ = nibble;
        nibbleCount_ = 1;
    } else {
        secondNibble_ = nibble;
        nibbleCount_ = 2;

        assembledByte_ = ((firstNibble_ & 0x0F) << 4) | (secondNibble_ & 0x0F);
        latestChar_ = static_cast<char>(assembledByte_);
        byteReady_ = true;
        completedCount_++;

        nibbleCount_ = 0;
    }
}

CharSnapshot CharReceiver::getSnapshot() {
    CharSnapshot snap;
    snap.currentNibble = secondNibble_;
    snap.assembledByte = assembledByte_;
    snap.byteReady = byteReady_;
    snap.latestChar = latestChar_;
    snap.completedCount = completedCount_;
    snap.nibbleCount = nibbleCount_;
    snap.resynced = resynced_;

    byteReady_ = false;
    resynced_ = false;

    return snap;
}