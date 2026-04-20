#include "../lib/CharReciever.h"

CharReceiver::CharReceiver()
    : firstNibble_(0),
      secondNibble_(0),
      nibbleCount_(0),
      assembledByte_(0),
      byteReady_(false),
      latestChar_('?'),
      completedCount_(0) {}

void CharReceiver::update(const PortBSnapshot& portSnap) {
    // Record nibbles whenever nibble changes, up to 2 per character
    if (portSnap.nibbleChanged) {
        if (nibbleCount_ == 0) {
            firstNibble_ = portSnap.nibble & 0x0F;
            nibbleCount_ = 1;
        } else if (nibbleCount_ == 1) {
            secondNibble_ = portSnap.nibble & 0x0F;
            nibbleCount_ = 2;
        } else {
            // If extra nibble changes happen before checkpoint,
            // keep the two most recent nibbles.
            firstNibble_ = secondNibble_;
            secondNibble_ = portSnap.nibble & 0x0F;
            nibbleCount_ = 2;
        }
    }

    // On checkpoint rising edge, finalize byte if 2 nibbles are available
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