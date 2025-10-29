// settings.cpp

#include "config.h"
#include "settings.h"
#include "gamepad.h"
#include <Arduino.h>
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

    uint32_t currentTime[9] = {
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	};

	uint32_t button_lastChangeTime[9] = {
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	};

	bool button_isPressed[9] = {
		false, false, false,
		false, false, false,
		false, false, false
	};

	bool currentRawState[9] = {
		false, false, false,
		false, false, false,
		false, false, false
	};

    uint8_t s_led_mode[4] = {99, 10, 20, 41};

    void processing();
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
        bool checkRelease = true;
        while (checkRelease) {
            for (int i = 0; i < 9; i++) {
                if (digitalRead(keys[i]) == HIGH) {
                    checkRelease = false;
                } else {
                    checkRelease = true;
                }
        }

        bool lastKeyState[9] = { false, false, false, false, false, false, false, false, false };

        uint32_t currentTime[9] = {
            0, 0, 0,
            0, 0, 0,
            0, 0, 0
        };

        uint32_t button_lastChangeTime[9] = {
            0, 0, 0,
            0, 0, 0,
            0, 0, 0
        };

        bool button_isPressed[9] = {
            false, false, false,
            false, false, false,
            false, false, false
        };

        bool currentRawState[9] = {
            false, false, false,
            false, false, false,
            false, false, false
        };


        bool Loop = true;
        bool saveTriggered = false;
        bool resetTriggered = false;

        while (Loop) {
            uint8_t start_lastCheckTime = 0;
            uint8_t start_lastCheckTime = 0;
            int32_t select_pressAccumulator = 0;
            int32_t select_pressAccumulator = 0;
            bool start_longPressTriggered = false;
            bool select_longPressTriggered = false;

            processing();
        }

        if (saveTriggered) {
            save();
        } else if (resetTriggered) {
            reset();
        } else {

        }
        
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

}

namespace {
    void processing() {
        for (int i = 0; i < 9; i++) {
			currentTime[i] = millis();
			currentRawState[i] = (digitalRead(keys[i]) == LOW);

			if (currentRawState[i] != button_isPressed[i]) {

				// デバウンスタイムが経過しているか確認
				if (currentTime[i] - button_lastChangeTime[i] > DEBOUNCE_DELAY_MS) {
					button_isPressed[i] = currentRawState[i];
					button_lastChangeTime[i] = currentTime[i];
				}
			}
		}

        for (int i = 0; i < 9; i++) {
			if (digitalRead(keys[i]) == LOW) {
				digitalWrite(keyLeds[i], HIGH);
			} 
			else {
				digitalWrite(keyLeds[i], LOW);
			}
		}

        // 鍵盤LEDモード変更 1鍵
        if (button_isPressed[0] && !lastKeyState[0]) { 
            if (settingsValue.keyboardLedMode < 3) {
                settingsValue.keyboardLedMode++;
            } else {
                settingsValue.keyboardLedMode = 0;
            }
            uint8_t num1 = (30 * settingsValue.keyboardLedMode) + 100;
            rp2040.fifo.push(num1);
            lastKeyState[0] = true;
        }

        // スクラッチLEDモード変更 2鍵
        if (button_isPressed[1] && !lastKeyState[1]) { 
            if (settingsValue.scratchLedMode >= 40 && settingsValue.scratchLedMode <= 42) {
                rp2040.fifo.push(s_led_mode[3]);
            } else if (settingsValue.scratchLedMode == 20) {
                rp2040.fifo.push(s_led_mode[2]);
            } else if (settingsValue.scratchLedMode == 10) {
                rp2040.fifo.push(s_led_mode[1]);
            } else {
                rp2040.fifo.push(s_led_mode[0]);
            }

            lastKeyState[1] = true;
        }

        // キーボードデバウンスタイム変更 3鍵
        if (button_isPressed[2] && !lastKeyState[2]) {
            uint8_t lastCount = MyGamepad::getAxisValue();
            while (digitalRead(keys[2]) == LOW) {
                uint8_t currCount = MyGamepad::getAxisValue();
                int8_t diff = (int8_t)(currCount - lastCount);
			    if (diff >= 1) { // 減少
                    if (settingsValue.debounceTime >= 2 && settingsValue.debounceTime >= 120) {
                        settingsValue.debounceTime -= 2;
                        rp2040.fifo.push(settingsValue.debounceTime / 2 + 100);
                        lastCount = currCount;
                    } else {
                        settingsValue.debounceTime = 120;
                        rp2040.fifo.push(settingsValue.debounceTime / 2 + 100);
                        lastCount = currCount;
                    }
                } else if (diff <= -1) {
                    if (settingsValue.debounceTime <= 118) {
                        settingsValue.debounceTime += 2;
                        rp2040.fifo.push(settingsValue.debounceTime / 2 + 100);
                        lastCount = currCount;
                    } else {
                        settingsValue.debounceTime = 120;
                        rp2040.fifo.push(settingsValue.debounceTime / 2 + 100); 
                        lastCount = currCount;
			        }
                }
            }
        }

        // スクラッチ感度変更 4鍵
        if (button_isPressed[3] && !lastKeyState[3]) {
            uint8_t lastCount = MyGamepad::getAxisValue();
            while (digitalRead(keys[3]) == LOW) {
                uint8_t currCount = MyGamepad::getAxisValue();
                int8_t diff = (int8_t)(currCount - lastCount);
                uint8_t sens = settingsValue.scratchSens / 3;
			    if (diff >= 1) { // 減少
                    if (sens >= 2 && sens <= 60)
                        sens--;;
                        settingsValue.scratchSens = sens * 3;
                        rp2040.fifo.push(sens + 100);
                        lastCount = currCount;
                    } else {
                        sens = 1;
                        settingsValue.scratchSens = 3;
                        rp2040.fifo.push(101);
                        lastCount = currCount;
                    }
                } else if (diff <= -1) {
                    if (sens >= 0 && sens < 60) {
                        sens++;
                        settingsValue.scratchSens = sens * 3;
                        rp2040.fifo.push(sens + 100);
                        lastCount = currCount;
                    } else {
                        sens = 60;
                        settingsValue.scratchSens = 60;
                        rp2040.fifo.push(160);
                        lastCount = currCount;
			    }
            }
        }

        // スクラッチLEDカラー変更 6鍵
        if (button_isPressed[5] && !lastKeyState[5]) {
            if (settingsValue.scratchLedColor < 18) {
                settingsValue.scratchLedColor++;
            } else {
                settingsValue.scratchLedColor = 0;
            }
            uint8_t num6 = 50 + settingsValue.scratchLedColor;
            rp2040.fifo.push(num6);
            lastKeyState[5] = true;
        }

        // スクラッチLED明るさ変更 7鍵
        if (button_isPressed[6] && !lastKeyState[6]) {
            uint8_t lastCount = MyGamepad::getAxisValue();
            while (digitalRead(keys[6]) == LOW) {
                uint8_t currCount = MyGamepad::getAxisValue();
                int8_t diff = (int8_t)(currCount - lastCount); 
                uint8_t inputVal = settingsValue.scratchLedBrightness;
                
                if (diff >= 1) { 
                    if (inputVal >= 2) {
                        inputVal--;
                        settingsValue.scratchLedBrightness = inputVal;
                        uint8_t num7 = 2000 + inputVal;
                        rp2040.fifo.push(num7);
                        uint8_t brightnessVal = map(inputVal, 1, 255, 1, 60);
                        rp2040.fifo.push(brightnessVal + 100);
                        lastCount = currCount;
                    } else {
                        inputVal = 1;
                        settingsValue.scratchLedBrightness = inputVal;
                        uint8_t num7 = 2000 + inputVal;
                        rp2040.fifo.push(num7);
                        uint8_t brightnessVal = map(inputVal, 1, 255, 1, 60);
                        rp2040.fifo.push(brightnessVal + 100);
                        lastCount = currCount;
                    }
                } else if (diff <= -1) {
                    if (inputVal <= 254) {
                        inputVal++;
                        settingsValue.scratchLedBrightness = inputVal;
                        uint8_t num7 = 2000 + inputVal;
                        rp2040.fifo.push(num7);
                        uint8_t brightnessVal = map(inputVal, 1, 255, 1, 60);
                        rp2040.fifo.push(brightnessVal + 100);
                        lastCount = currCount;
                    } else {
                        inputVal = 255;
                        settingsValue.scratchLedBrightness = inputVal;
                        uint8_t num7 = 2000 + inputVal;
                        rp2040.fifo.push(num7);
                        uint8_t brightnessVal = map(inputVal, 1, 255, 1, 60);
                        rp2040.fifo.push(brightnessVal + 100);
                        lastCount = currCount;
                    }
                }
            }
        }

        // 保存 START 
		uint32_t start_now = millis();
		uint32_t start_elapsedTime = start_now - start_lastCheckTime;
		start_lastCheckTime = start_now;

		if (digitalRead(keys[7]) == LOW) {
			start_pressAccumulator += start_elapsedTime;
			if (start_pressAccumulator > LONG_PRESS_DURATION) {
				start_pressAccumulator = LONG_PRESS_DURATION;
			}
		} else {
			start_pressAccumulator -= start_elapsedTime;
			if (start_pressAccumulator < 0) {
			start_pressAccumulator = 0;
			}
		}

		if (start_pressAccumulator >= LONG_PRESS_DURATION && !start_longPressTriggered) {
			Loop = false;
            saveTriggered = true;
			start_pressAccumulator = 0;
			start_longPressTriggered = true;
		}

		if (start_pressAccumulator == 0) {
			start_longPressTriggered = false;
		}

        // リセット SELECT
        uint32_t select_now = millis();
		uint32_t select_elapsedTime = select_now - select_lastCheckTime;
		select_lastCheckTime = select_now;

		if (digitalRead(keys[8]) == LOW) {
			select_pressAccumulator += select_elapsedTime;
			if (select_pressAccumulator > LONG_PRESS_DURATION) {
				select_pressAccumulator = LONG_PRESS_DURATION;
			}
		} else {
			select_pressAccumulator -= select_elapsedTime;
			if (select_pressAccumulator < 0) {
			select_pressAccumulator = 0;
			}
		}

		if (select_pressAccumulator >= LONG_PRESS_DURATION && !select_longPressTriggered) {
			Loop = false;
            resetTriggered = true;
			select_pressAccumulator = 0;
			select_longPressTriggered = true;
		}

		if (select_pressAccumulator == 0) {
			select_longPressTriggered = false;
		}
		


        for (int i = 0; i < 9; i++) {
            if (digitalRead(keys[i]) == HIGH) {
                    lastKeyState[i] = false;
            }
        }
    }
}