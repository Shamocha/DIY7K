// config.h

#pragma once
#include <Arduino.h>

// USB VID/PID
constexpr uint16_t USB_VID = 0xCafe;
constexpr uint16_t USB_PID = 0x4001;

// Keyboard Pins
constexpr uint8_t KEY1_PIN = 2;
constexpr uint8_t KEY2_PIN = 3;
constexpr uint8_t KEY3_PIN = 4;
constexpr uint8_t KEY4_PIN = 5;
constexpr uint8_t KEY5_PIN = 6;
constexpr uint8_t KEY6_PIN = 7;
constexpr uint8_t KEY7_PIN = 8;

constexpr uint8_t START_PIN = 9;
constexpr uint8_t SELECT_PIN = 10;


// Scratch Encoder Pins
constexpr uint8_t ENCODER_PIN_A = 11; // B相は A+1


// Keyboard LED Pins
constexpr uint8_t KEY1_LED_PIN = 14;
constexpr uint8_t KEY2_LED_PIN = 15;
constexpr uint8_t KEY3_LED_PIN = 16;
constexpr uint8_t KEY4_LED_PIN = 17;
constexpr uint8_t KEY5_LED_PIN = 18;
constexpr uint8_t KEY6_LED_PIN = 19;
constexpr uint8_t KEY7_LED_PIN = 20;
constexpr uint8_t KEY8_LED_PIN = 21;
constexpr uint8_t KEY9_LED_PIN = 22;


// Scratch LED Pin
constexpr uint8_t SCRATCH_LED_PIN = 24;

constexpr uint8_t DEFAULT_SCRATCH_LED_BRIGHTNESS = 50;


// Debounce time (ms)
constexpr uint8_t DEFAULT_DEBOUNCE_DELAY_MS = 30;

constexpr uint8_t DEFAULT_ENCODER_SENS = 60;


/*
// Keyboard Keycodes
constexpr uint8_t KEY1_CODE 1
constexpr uint8_t KEY2_CODE 2
constexpr uint8_t KEY3_CODE 3
constexpr uint8_t KEY4_CODE 4
constexpr uint8_t KEY5_CODE 5
constexpr uint8_t KEY6_CODE 6
constexpr uint8_t KEY7_CODE 7

constexpr uint8_t START_CODE 8
constexpr uint8_t SELECT_CODE 9
*/



// Do not modify below this line
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

/*
keymaps[9] = {
    KEY1_CODE,
    KEY2_CODE,
    KEY3_CODE,
    KEY4_CODE,
    KEY5_CODE,
    KEY6_CODE,
    KEY7_CODE,
    START_CODE,
    SELECT_CODE
}
    */