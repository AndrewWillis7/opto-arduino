#include <Arduino.h>
#include "../lib/Configuration.h"
#include "../lib/Blinker.h"


// need to build the linker-driven project files
// Something like this:

/*
project/
├── platformio.ini
├── src/
│   ├── common/
│   │   ├── shared.cpp
│   │   └── shared.h
│   ├── mcu_a/
│   │   └── main.cpp
│   └── mcu_b/
│       └── main.cpp
└── lib/
*/


using namespace Hardware;

Blinker blinker(timer);

void setup() {
  Serial.begin(baudrate);
  Serial.println("Initialized");

  pinMode(SENS, INPUT);
  DDRB |= (1 << 4);
}

void loop() {
  if (blinker.update(micros())) {
    PORTB ^= (1 << 4);
  }

  uint8_t lightRead = digitalRead(SENS) ? 0 : 1;
  Serial.println(lightRead);
}