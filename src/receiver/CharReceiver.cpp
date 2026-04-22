#include "../lib/CharReceiver.h"

CharReceiver::CharReceiver()
    : firstNibble_(0),
      secondNibble_(0),
      nibbleCount_(0),
      assembledByte_(0),
      byteReady_(false),
      latestChar_('?'),
      completedCount_(0) {}

void CharReceiver::update(const PortBSnapshot& portSnap) {
    // First, capture nibble changes
    if (portSnap.nibbleChanged) {
        if (nibbleCount_ == 0) {
            firstNibble_ = portSnap.nibble & 0x0F;
            nibbleCount_ = 1;
        } else if (nibbleCount_ == 1) {
            secondNibble_ = portSnap.nibble & 0x0F;
            nibbleCount_ = 2;
        } else {
            // Ignore extras once we already have a full byte candidate
            // until checkpoint commits it.
        }
    }

    // Then, commit only if a full byte is ready
    if (portSnap.checkpointRising && nibbleCount_ == 2) {
        assembledByte_ = ((firstNibble_ & 0x0F) << 4) | (secondNibble_ & 0x0F);
        latestChar_ = static_cast<char>(assembledByte_);
        byteReady_ = true;
        completedCount_++;

        // Reset for next character
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

    byteReady_ = false;
    return snap;
}