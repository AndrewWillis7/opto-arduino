#include <Arduino.h>
#include <unity.h>
#include "../lib/Blinker.h"

Blinker blinker(10000);

void test_no_toggle_before_interval() {
    bool toggled = blinker.update(5000);
    TEST_ASSERT_FALSE(toggled);
}

void test_toggle_at_interval() {
    blinker.update(0);
    bool toggled = blinker.update(10000);
    TEST_ASSERT_TRUE(toggled);
}

