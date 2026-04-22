#include "../lib/receiver_app.h"
#include "../lib/Configuration.h"

using namespace Hardware;

App::App()
    : reader_(),
      receiver_(),
      view_(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7),
      lastRefreshMs_(0) {}

void App::begin() {
    Serial.begin(9600);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    reader_.begin(false, false, false);
    view_.begin();

    Serial.println("Receiver Ready");
}

void App::update() {
    PortBSnapshot portSnap = reader_.getSnapshot();

    if (portSnap.nibbleChanged || portSnap.checkpointRising) {
        receiver_.update(portSnap);
    }

    CharSnapshot charSnap = receiver_.getSnapshot();

    /*
    CHARACTER SUCCESS
    */
    if (charSnap.byteReady) {

        // Blink green LED
        digitalWrite(LED_GREEN, HIGH);
        delay(40);
        digitalWrite(LED_GREEN, LOW);

        // Print received char to console
        Serial.print(charSnap.latestChar);
    }

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