// config.h

#pragma once
#include <Arduino.h>

// USB VID/PID
constexpr uint16_t USB_VID = 0xCAFE;
constexpr uint16_t USB_PID = 0xF00D;

// Keyboard Pins
constexpr uint8_t KEY1_PIN = 2;
constexpr uint8_t KEY2_PIN = 3;
constexpr uint8_t KEY3_PIN = 4;
constexpr uint8_t KEY4_PIN = 5;
constexpr uint8_t KEY5_PIN = 6;
constexpr uint8_t KEY6_PIN = 7;
constexpr uint8_t KEY7_PIN = 8;

constexpr uint8_t START_PIN = 10;
constexpr uint8_t SELECT_PIN = 11;


// Scratch Encoder Pins
constexpr uint8_t ENCODER_PIN_A = 12; // B相は A+1


// Keyboard LED Pins
constexpr uint8_t KEY1_LED_PIN = 16;
constexpr uint8_t KEY2_LED_PIN = 17;
constexpr uint8_t KEY3_LED_PIN = 18;
constexpr uint8_t KEY4_LED_PIN = 19;
constexpr uint8_t KEY5_LED_PIN = 20;
constexpr uint8_t KEY6_LED_PIN = 21;
constexpr uint8_t KEY7_LED_PIN = 22;
constexpr uint8_t KEY8_LED_PIN = 14;
constexpr uint8_t KEY9_LED_PIN = 15;

constexpr uint8_t LED_MOVE_INTERVAL = 100;


// Scratch LED Settings
constexpr uint8_t SCRATCH_LED_PIN = 27;
constexpr uint8_t NUM_PIXELS = 60; // 変えても多分意味ない　ところどころ数値を直接書いてる


constexpr uint8_t DEFAULT_SCRATCH_LED_BRIGHTNESS = 50;
constexpr uint8_t DEFAULT_LED_COLOR = 0;
constexpr uint8_t DEFAULT_SCRATCH_LED_MODE = 10;

constexpr uint16_t LIGHTS_TIME = 350;


// Other

constexpr uint8_t DEFAULT_KEYBOARD_LED_MODE = 2;

constexpr uint8_t DEFAULT_DEBOUNCE_DELAY_MS = 30;

constexpr uint8_t DEFAULT_ENCODER_SENS = 60;

constexpr int32_t LONG_PRESS_DURATION = 3000; 


// Do not modify below this line
// 以下のコードを変更してはいけない
constexpr uint8_t keys[9] = {
    KEY1_PIN,
    KEY2_PIN,
    KEY3_PIN,
    KEY4_PIN,
    KEY5_PIN,
    KEY6_PIN,
    KEY7_PIN,
    START_PIN,
    SELECT_PIN
};

constexpr uint8_t keyLeds[9] = {
    KEY1_LED_PIN,
    KEY2_LED_PIN,
    KEY3_LED_PIN,
    KEY4_LED_PIN,
    KEY5_LED_PIN,
    KEY6_LED_PIN,
    KEY7_LED_PIN,
    KEY8_LED_PIN,
    KEY9_LED_PIN
};
