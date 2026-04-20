#include "../lib/reciever_app.h"

App::App()
    : reader_(),
      receiver_(),
      view_(2, 3, 4, 5, 6, 7),
      lastRefreshMs_(0) {}

void App::begin() {
    reader_.begin(false, false, false);
    view_.begin();
}

void App::update() {
    PortBSnapshot portSnap = reader_.getSnapshot();

    if (portSnap.nibbleChanged || portSnap.checkpointRising) {
        receiver_.update(portSnap);
    }

    CharSnapshot charSnap = receiver_.getSnapshot();

    unsigned long now = millis();
    if ((now - lastRefreshMs_) >= refreshPeriodMs_ ||
        portSnap.nibbleChanged ||
        portSnap.checkpointRising ||
        charSnap.byteReady) {

        lastRefreshMs_ = now;

        view_.show(
            portSnap.nibble,
            charSnap.assembledByte,
            charSnap.latestChar,
            charSnap.completedCount,
            charSnap.nibbleCount
        );
    }
}