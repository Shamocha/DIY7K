#pragma once

#include <Arduino.h>

namespace MyGamepad {
    void gamepad_setup();
    void gamepad_loop();
    void keyLighting_flow();
    uint8_t getAxisValue();
} 