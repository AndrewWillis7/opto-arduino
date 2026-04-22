#pragma once
#include <Arduino.h>

namespace Hardware {
    constexpr unsigned long baudrate = 9600;

    // Receiver input pins (PORTB on Uno)
    constexpr uint8_t RX_BIT0 = 10;   // PB0
    constexpr uint8_t RX_BIT1 = 11;   // PB1
    constexpr uint8_t RX_BIT2 = 12;  // PB2
    constexpr uint8_t RX_BIT3 = 13;  // PB3
    constexpr uint8_t RX_CHECK = 9; // PB4
    constexpr uint8_t LED_GREEN = 9;

    // LCD wiring: RS, E, D4, D5, D6, D7
    constexpr uint8_t LCD_RS = 1;
    constexpr uint8_t LCD_E  = 0;
    constexpr uint8_t LCD_D4 = 6;
    constexpr uint8_t LCD_D5 = 5;
    constexpr uint8_t LCD_D6 = 4;
    constexpr uint8_t LCD_D7 = 3;
}