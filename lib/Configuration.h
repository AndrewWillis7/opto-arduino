#pragma once

namespace Hardware {
    // Generic Configuration
    #define baudrate 9600

    // Single Pin Definitions
    #define LED 12
    #define SENS 13

    // Input
    #define timer 10000

    // Light Reader Pins
    #define L1 8;
    #define L2 9;
    #define L3 10;
    #define L4 11;

    // LCD Wiring
    // RS, E, D4, D5, D6, D7
    int lcd_pins[6] = {2, 3, 4, 5, 6, 7};
} // EON
