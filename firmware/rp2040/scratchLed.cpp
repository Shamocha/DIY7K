#include "scratchLed.h"
#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, SCRATCH_LED_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// 10 = rainbow cycle, 20 = constant color
// 41 = left half, 42 = right half
// 50 to 70 = selected color
// 99 = off
// 100 to 160 = number of pixels lit
// 2000 to 2255 = brightness level (0 to 255)

namespace {

	Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, SCRATCH_LED_PIN, NEO_GRB + NEO_KHZ800);

	uint8_t SCRATCH_LED_BRIGHTNESS = DEFAULT_SCRATCH_LED_BRIGHTNESS;
	uint8_t SCRATCH_LED_MODE = DEFAULT_SCRATCH_LED_MODE;
	uint8_t SCRATCH_LED_COLOR = DEFAULT_LED_COLOR;
	uint8_t SCRATCH_LED_PIXELS = 0;
	uint16_t PixelsLightDuration = 500;
	int pixels_timer = 0;

	bool data_received = false;

	bool halfon = false;
	bool left_on = false;
	bool right_on = false;

	uint32_t left_timer = 0;
	uint32_t right_timer = 0;

	uint32_t rbc_update_interval = 50;
	uint32_t rbc_last_update_time = 0;
	uint16_t rbc_time_offset = 0;

	struct RawColorData {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	uint32_t red = strip.Color(255, 0, 0);
	uint32_t green = strip.Color(0, 255, 0);
	uint32_t blue = strip.Color(0, 0, 255);
	uint32_t cyan = strip.Color(0, 255, 255);
	uint32_t purple = strip.Color(255, 0, 255);
	uint32_t yellow = strip.Color(255, 255, 0);
	uint32_t white = strip.Color(255, 255, 255);

	uint32_t pink = strip.Color(255, 105, 180);  
	uint32_t salmon = strip.Color(250, 128, 114); 
	uint32_t orange = strip.Color(255, 165, 0); 
	uint32_t gold = strip.Color(255, 215, 0);  
	uint32_t brown = strip.Color(165, 42, 42);    

	uint32_t teal = strip.Color(0, 128, 128);     
	uint32_t lime = strip.Color(191, 255, 0);     
	uint32_t indigo = strip.Color(75, 0, 130);     
	uint32_t violet = strip.Color(238, 130, 238);  
	uint32_t navy = strip.Color(0, 0, 128);      

	uint32_t silver = strip.Color(192, 192, 192);  
	uint32_t gray = strip.Color(128, 128, 128);   

	uint32_t off = strip.Color(0, 0, 0);

	uint32_t colorArray[20] = {
		red, green, blue, cyan, purple,
		yellow, white, pink, salmon, orange, gold,
		brown, teal, lime, indigo, violet,
		navy, silver, gray,
		off
	};
		
	RawColorData allColors[20] = {
		{255, 0, 0},    // Red
		{0, 255, 0},    // Green
		{0, 0, 255},    // Blue
		{0, 255, 255},  // Cyan
		{255, 0, 255},  // Purple
		{255, 255, 0},  // Yellow
		{255, 255, 255},// White
		{255, 105, 180},// Pink
		{250, 128, 114},// Salmon
		{255, 165, 0},  // Orange
		{255, 215, 0},  // Gold
		{165, 42, 42},  // Brown
		{0, 128, 128},  // Teal
		{191, 255, 0},  // Lime
		{75, 0, 130},   // Indigo
		{238, 130, 238},// Violet
		{0, 0, 128},    // Navy
		{192, 192, 192},// Silver
		{128, 128, 128},// Gray
		{0, 0, 0}       // Off
	};


	void halfLit();
	uint32_t Wheel(byte WheelPos);
	void myRainbowCircle();

}

namespace MyScratchLed {

	void led_setup() {

		strip.begin();
		strip.setBrightness(SCRATCH_LED_BRIGHTNESS);
		strip.show(); // Initialize all pixels to 'off'

	}

	void led_loop() {

		uint32_t latest_data = 0;
		data_received = false;

		while (rp2040.fifo.available()) {
			latest_data = rp2040.fifo.pop();
			data_received = true;

			if (latest_data >= 2000 && latest_data <= 2255) {
				SCRATCH_LED_BRIGHTNESS = (uint8_t)(latest_data - 2000);
				strip.setBrightness(SCRATCH_LED_BRIGHTNESS);
			} 

			if (latest_data >= 10 && latest_data <= 20) {
				SCRATCH_LED_MODE = (uint8_t)(latest_data);
			}

			if (latest_data >= 50 && latest_data <= 69) {
				SCRATCH_LED_COLOR = (uint8_t)(latest_data - 50);
			}

			if (SCRATCH_LED_MODE >= 40 && SCRATCH_LED_MODE <= 42) {
				SCRATCH_LED_MODE = (uint8_t)(latest_data);
				halfon = true;
			}

			if (latest_data == 99) {
				SCRATCH_LED_MODE = 99;
			}

			if (latest_data >= 100 && latest_data <= 160) {
				SCRATCH_LED_PIXELS = (uint8_t)(latest_data - 100);
				pixels_timer = millis();
			}

		}

		uint32_t currTime = millis();
		if (currTime - pixels_timer < PixelsLightDuration) {
			strip.clear();
			strip.fill(colorArray[SCRATCH_LED_COLOR], 0, SCRATCH_LED_PIXELS);
			strip.show();
		} else {

			if (SCRATCH_LED_MODE == 10) {
				myRainbowCircle();
			}

			if (SCRATCH_LED_MODE == 20) {
				strip.fill(colorArray[SCRATCH_LED_COLOR], 0, strip.numPixels());
				strip.show();
			}

			if (SCRATCH_LED_MODE >= 40 && SCRATCH_LED_MODE <= 42) {
				halfLit();
			}

			if (SCRATCH_LED_MODE == 99) {
				strip.fill(0, 0, strip.numPixels());
				strip.show();
			}
		}



	}

}


namespace {

	void halfLit() {

		if (SCRATCH_LED_MODE == 41 && halfon) {
		strip.fill(colorArray[SCRATCH_LED_COLOR], 0, strip.numPixels() / 2);
		strip.fill(0, strip.numPixels() / 2, strip.numPixels() / 2);
		strip.show();
		halfon = false;
		left_on = true;
		right_on = false;
		SCRATCH_LED_MODE = 40;
		right_timer = 0;
		left_timer = millis();
	}

	if (SCRATCH_LED_MODE == 42 && halfon) {
		strip.fill(0, 0, strip.numPixels() / 2);
		strip.fill(colorArray[SCRATCH_LED_COLOR], strip.numPixels() / 2, strip.numPixels() / 2);
		strip.show();
		halfon = false;
		left_on = false;
		right_on = true;
		left_timer = 0;
		SCRATCH_LED_MODE = 40;
		right_timer = millis();
	}

	if (left_on) {
		uint32_t base_time = millis();
		if (base_time - left_timer < LIGHTS_TIME) {
			float ratio = (float)(base_time - left_timer) / LIGHTS_TIME;
			uint8_t new_red = (uint8_t)(allColors[SCRATCH_LED_COLOR].red * ratio);
			uint8_t new_green = (uint8_t)(allColors[SCRATCH_LED_COLOR].green * ratio);
			uint8_t new_blue = (uint8_t)(allColors[SCRATCH_LED_COLOR].blue * ratio);
			uint32_t new_color = strip.Color(new_red, new_green, new_blue);
			strip.fill(new_color, 0, strip.numPixels() / 2);
			strip.fill(0, strip.numPixels() / 2, strip.numPixels() / 2);
			strip.show();
		} else {
			left_on = false;
		}
	}

	if (right_on) {
		uint32_t base_time = millis();
		if (base_time - right_timer < LIGHTS_TIME) {
			float ratio = (float)(base_time - right_timer) / LIGHTS_TIME;
			uint8_t new_red = (uint8_t)(allColors[SCRATCH_LED_COLOR].red * ratio);
			uint8_t new_green = (uint8_t)(allColors[SCRATCH_LED_COLOR].green * ratio);
			uint8_t new_blue = (uint8_t)(allColors[SCRATCH_LED_COLOR].blue * ratio);
			uint32_t new_color = strip.Color(new_red, new_green, new_blue);
			strip.fill(0, 0, strip.numPixels() / 2);
			strip.fill(new_color, strip.numPixels() / 2, strip.numPixels() / 2);
			strip.show();
		} else {
			right_on = false;
		}
	}
}

	void myRainbowCircle() {

		if (millis() - rbc_last_update_time < rbc_update_interval) {
			return; 
		}

		rbc_last_update_time = millis();

		for (uint16_t i = 0; i < strip.numPixels(); i++) {
			
			uint32_t color = Wheel(((i * 256 / strip.numPixels()) + rbc_time_offset) & 255);
			strip.setPixelColor(i, color);
		}

		strip.show();

		rbc_time_offset++;

		if (rbc_time_offset >= (256 * 5)) {
			rbc_time_offset = 0;
		}

	}


	uint32_t Wheel(byte WheelPos) {
		WheelPos = 255 - WheelPos;
		if(WheelPos < 85) {
			return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
		}
		if(WheelPos < 170) {
			WheelPos -= 85;
			return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
		}
		WheelPos -= 170;
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}

}