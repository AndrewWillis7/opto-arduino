#include "../lib/receiver_app.h"
#include "../lib/Configuration.h"

using namespace Hardware;

App::App()
    : reader_(),
      receiver_(),
      view_(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7),
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