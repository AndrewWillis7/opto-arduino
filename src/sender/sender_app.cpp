#include "../lib/sender_app.h"

SenderApp::SenderApp()
    : protocol_(), sentOnce_(false) {}

void SenderApp::begin() {
    protocol_.begin();
}

void SenderApp::update() {
    protocol_.sendString("Hello World!");
    delay(1000);
}