#pragma once
#include <Arduino.h>
#include "../lib/NibbleReader.h"
#include "../lib/CharReceiver.h"
#include "../lib/LCDView.h"

class App {
public:
    App();
    void begin();
    void update();

private:
    NibbleReader reader_;
    CharReceiver receiver_;
    LcdView view_;

    unsigned long lastRefreshMs_;
    static constexpr unsigned long refreshPeriodMs_ = 50;
};