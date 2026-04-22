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
    if (portSnap.checkpointRising) {
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