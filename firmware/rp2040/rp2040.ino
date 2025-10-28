#include <Arduino.h>
#include "gamepad.h"
#include "scratchLed.h"
#include "settings.h"

bool core0_not_ready = true;
bool core1_not_ready = true;
uint32_t core0_send_data = 0;
uint32_t core0_received_data = 0;
uint32_t core1_received_data = 0;

void setup() {

    MySettings::load();

    MyGamepad::gamepad_setup();
    core0_send_data = 500;
    rp2040.fifo.push(core0_send_data);

    while (core1_not_ready) {
        if (rp2040.fifo.available()) {
            core0_received_data = rp2040.fifo.pop();
        }

        if (core0_received_data == 600) {
            core1_not_ready = false;
        }
    }

    

}


void loop() {
    MyGamepad::gamepad_loop();
}


void setup1() {

    while (core0_not_ready) {

        if (rp2040.fifo.available()) {
          core1_received_data = rp2040.fifo.pop();
        }

        if (core1_received_data == 500) {
          core0_not_ready = false;
        }
    }
    
    MyScratchLed::led_setup();

    rp2040.fifo.push(600); // ready
}

void loop1() {
    MyScratchLed::led_loop();
}