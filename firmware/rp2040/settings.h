// settings.h

#pragma once

#include <Arduino.h>

namespace MySettings {

    void load();
    void changeMode();
    void save();
    void reset();
    void to_core1_led_setup();


    bool getIsEnabled();
    bool getIsMagicValid();
    uint8_t getKeyboardLedMode();
    uint8_t getDebounceTime();
    uint8_t getScratchSens();
    uint8_t getScratchLedMode();
    uint8_t getScratchLedColor();
    uint8_t getScratchLedBrightness();

}