#include "../lib/sender_app.h"

SenderApp::SenderApp()
    : protocol_(), sentOnce_(false) {}

void SenderApp::begin() {
    protocol_.begin();
}

void SenderApp::update() {
    if (!sentOnce_) {
        protocol_.sendString("Hello World!");
        sentOnce_ = true;
    }

    delay(1000);

    protocol_.sendString("Hello World!");
}