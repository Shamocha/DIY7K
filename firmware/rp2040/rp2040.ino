#include <Arduino.h>
#include "gamepad.h"
#include "scratchLed.h"

void setup() {
    MyGamepad::gamepad_setup();
}


void loop() {
    MyGamepad::gamepad_loop();
}


void setup1() {
    MyScratchLed::led_setup();
}

void loop1() {
    MyScratchLed::led_loop();
}