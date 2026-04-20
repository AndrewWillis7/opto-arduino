#pragma once
#include <Arduino.h>
#include "../src/sender/SenderProtocol.h"

class SenderApp {
public:
    SenderApp();
    void begin();
    void update();

private:
    SenderProtocol protocol_;
    bool sentOnce_;
};