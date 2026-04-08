#include "../lib/Configuration.h"
#include "../lib/reciever_app.h"

using namespace Hardware;

App::App()
    :   reader_(),
        view_(lcd_pins[1], lcd_pins[2], lcd_pins[3],
            lcd_pins[4], lcd_pins[5], lcd_pins[6]),
        lastRefreshMs_(0) {}

void App::begin() {
    // set the second argument to true if logic is inverted
    reader_.begin(false, true);
    view_.begin();
}

void App::update() {
    unsigned long now = millis();
    bool changed = reader_.hasChanged();

    if (changed || (now - lastRefreshMs_ >= refreshPeriodMs_)) {
        lastRefreshMs_ = now;

        uint8_t nibble = reader_.getLatestNibble();
        uint32_t count = reader_.getChangeCount();

        view_.showNibble(nibble, count);
    }
}