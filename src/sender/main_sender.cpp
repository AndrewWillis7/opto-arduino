#include <Arduino.h>
#include "../lib/sender_app.h"

SenderApp app;

void setup() {
    app.begin();
}

void loop() {
    app.update();
}