#include "../lib/receiver_app.h"
#include "../lib/Configuration.h"
#include <avr/wdt.h>

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

    reader_.begin(false, true, true);

    // Timeout is slightly shorter than the gap between repeated message bursts.
    // Sender timing per nibble is huge right now, so 1200 ms is safe.
    receiver_.begin(1200);

    view_.begin();

    // Clean start for watchdog
    wdt_disable();
    wdt_enable(WDTO_2S);

    Serial.println("Receiver Ready");
}

void App::update() {
    // Feed the watchdog once per main loop
    wdt_reset();

    PortBSnapshot portSnap = reader_.getSnapshot();

    if (portSnap.checkpointRising) {
        receiver_.update(portSnap);
    }

    CharSnapshot charSnap = receiver_.getSnapshot();

    if (charSnap.resynced) {
        Serial.println("\n[RESYNC] Checkpoint timeout detected, restarting byte assembly.");
    }

    if (charSnap.byteReady) {
        digitalWrite(LED_GREEN, HIGH);
        delay(40);
        digitalWrite(LED_GREEN, LOW);

        Serial.print(charSnap.latestChar);

        // Helpful debug line if you want to inspect weird bytes:
        Serial.print("  [0x");
        if (charSnap.assembledByte < 0x10) Serial.print('0');
        Serial.print(charSnap.assembledByte, HEX);
        Serial.println("]");
    }

    unsigned long now = millis();
    if ((now - lastRefreshMs_) >= refreshPeriodMs_ ||
        portSnap.nibbleChanged ||
        portSnap.checkpointRising ||
        charSnap.byteReady ||
        charSnap.resynced) {

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