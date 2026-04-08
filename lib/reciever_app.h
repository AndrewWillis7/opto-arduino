#pragma once
#include <Arduino.h>
#include "./NibbleReader.h"
#include "./LCDView.h"

class App {
public:
    App();

    void begin();
    void update();

private:
    NibbleReader reader_;
    LCDView view_;

    unsigned long lastRefreshMs_;
    static constexpr unsigned long refreshPeriodMs_ = 50;
};