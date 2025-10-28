// settings.h

#pragma once

namespace MySettings {

    void load();
    void changeMode();
    void save();
    void core1_setup();
    void fifo_push(uint32_t data);


    bool getIsEnabled();
    bool getIsMagicValid();
    uint8_t getDebounceTime();
    uint8_t getScratchSens();
    uint8_t getScratchLedMode();
    uint8_t getScratchLedBrightness();

}