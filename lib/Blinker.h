#pragma once
#include <stdint.h>

class Blinker {
public:
    Blinker(unsigned long interval_us)
        : interval(interval_us), previousMicros(0), state(false) {}

    // returns true if toggle should happen
    bool update(unsigned long now) {
        if (now - previousMicros >= interval) {
            previousMicros = now;
            state = !state;
            return true;
        }
        return false;
    }

    bool getState() const {
        return state;
    }

private:
    unsigned long interval;
    unsigned long previousMicros;
    bool state;
};