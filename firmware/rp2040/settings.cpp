// settings.cpp

#include "config.h"
#include "settings.h"
#include <EEPROM.h>

namespace {

    constexpr uint32_t EEPROM_MAGIC = 0xCAFE1234;
    constexpr int EEPROM_ADDR = 0;

    struct MyConfig {
        uint32_t magic;     
        bool     isEnabled;
        bool     isMagicValid;
        uint8_t  keyboardLedMode; // 0 = 常時消灯, 1 = 常時点灯, 2 = 押下時点灯, 3 = ACライク
        uint8_t  debounceTime;
        uint8_t  scratchSens;
        uint8_t  scratchLedMode;
        uint8_t  scratchLedColor; 
        uint8_t  scratchLedBrightness;
    };

    Myconfig settingsValue;
}

namespace MySettings {

    void load() {

        EEPROM.begin(sizeof(MyConfig));
        EEPROM.get(EEPROM_ADDR, settingsValue);

        settingsValue.isEnabled = false;
        settingsValue.isMagicValid = false;

        if (settingsValue.magic != EEPROM_MAGIC) {
            settingsValue.isEnabled = true;
            settingsValue.isMagicValid = false;
            settingsValue.keyboardLedMode = DEFAULT_KEYBOARD_LED_MODE;
            settingsValue.debounceTime = DEFAULT_DEBOUNCE_DELAY_MS;
            settingsValue.scratchSens = DEFAULT_ENCODER_SENS;
            settingsValue.scratchLedMode = DEFAULT_SCRATCH_LED_MODE;
            settingsValue.scratchLedColor = DEFAULT_LED_COLOR;
            settingsValue.scratchLedBrightness = DEFAULT_SCRATCH_LED_BRIGHTNESS;
            

        } else {
            settingsValue.isEnabled = true;
            settingsValue.isMagicValid = true;
        }
    }

    void changeMode() {
        
    }

    void save() {
        // Placeholder for future settings changes
    }

    void core1_led_setup() {

        rp2040.fifo.push((uint32_t)settingsValue.scratchLedMode);
                
        uint32_t brightness = 2000 + settingsValue.scratchLedBrightness;
        rp2040.fifo.push(brightness);
                
        uint32_t color = 50 + settingsValue.scratchLedColor;
        rp2040.fifo.push(color);


    }

    void fifo_push(uint32_t data) {
        rp2040.fifo.push(data);
    }

    bool getIsEnabled() {
        return settingsValue.isEnabled;
    }

    bool getIsMagicValid() {
        return settingsValue.isMagicValid;
    }

    uint8_t getKeyboardLedMode() {
        return settingsValue.keyboardLedMode;
    }

    uint8_t getDebounceTime() {
        return settingsValue.debounceTime;
    }

    uint8_t getScratchSens() {
        return settingsValue.scratchSens;
    }

    uint8_t getScratchLedMode() {
        return settingsValue.scratchLedMode;
    }

    uint8_t getScratchLedColor() {
        return settingsValue.scratchLedColor;
    }

    uint8_t getScratchLedBrightness() {
        return settingsValue.scratchLedBrightness;
    }

}