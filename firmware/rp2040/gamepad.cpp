#include "gamepad.h"
#include <Arduino.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "Adafruit_TinyUSB.h"
// #include <Adafruit_NeoPixel.h>
#include "quadrature_encoder.pio.h"
#include "config.h"
#include "settings.h"


namespace {

	uint8_t DEBOUNCE_DELAY_MS = 30;
	uint8_t ENCODER_SENS = 60;
	uint8_t keyLedMode = 0;

	uint32_t lastCheckTime = 0;
	int32_t pressAccumulator = 0;
	bool longPressTriggered = false;

	uint32_t lastLedMoveTime = 0; 
	int8_t currLed = 0;           
	int8_t ledDirection = 1;

	bool firstLoop = true;
	uint8_t lastAxisValue = 0;

	
	PIO g_pio_enc_instance = pio0; // PIOインスタンス
	uint g_pio_enc_sm;            // ステートマシン番号

	uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_GAMEPAD()
	};

	Adafruit_USBD_HID usb_hid;

	hid_gamepad_report_t gp;

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

	bool setupPioEncoder(uint pin_a);
	void reset_gamepad_report();
	void settings_check_buttonState();
	

}

namespace MyGamepad {
	void gamepad_setup() {

		// GPIO setup for keys
		for (int i = 0; i < 9; i++) {
			pinMode(keys[i], INPUT_PULLUP);
		}

		// GPIO setup for key LEDs
		for (int i = 0; i < 9; i++) {
			pinMode(keyLeds[i], OUTPUT);
			digitalWrite(keyLeds[i], LOW);
		}

		if (!TinyUSBDevice.isInitialized()) {
			TinyUSBDevice.begin(0);
		}

		Serial.begin(115200);

		

		// Setup HID
		TinyUSBDevice.setID(USB_VID, USB_PID);
		usb_hid.setPollInterval(1);
		usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
		usb_hid.begin();

		// If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
		if (TinyUSBDevice.mounted()) {
			TinyUSBDevice.detach();
			delay(10);
			TinyUSBDevice.attach();
		}

		Serial.println("TinyUSB Initialized.");

		// Encoder setup

		if (!setupPioEncoder(ENCODER_PIN_A)) {
			Serial.println("PIO Encoder Init Failed.");
			while (true); 
		}
		
		Serial.println("PIO Encoder Initialized.");


		// 設定読み込み
		if (MySettings::getIsEnabled()) {
			if (MySettings::getIsMagicValid()) {
				DEBOUNCE_DELAY_MS = MySettings::getDebounceTime();
				ENCODER_SENS = MySettings::getScratchSens();
				keyLedMode = MySettings::getKeyboardLedMode();
			} else {
				DEBOUNCE_DELAY_MS = DEFAULT_DEBOUNCE_DELAY_MS;
				ENCODER_SENS = DEFAULT_ENCODER_SENS;
			}
		} else {
			DEBOUNCE_DELAY_MS = DEFAULT_DEBOUNCE_DELAY_MS;
			ENCODER_SENS = DEFAULT_ENCODER_SENS;
		}

		Serial.println("All Gamepad Task Initialized.");

	}

	void gamepad_loop() {
		keyLedMode = MySettings::getKeyboardLedMode();
		DEBOUNCE_DELAY_MS = MySettings::getDebounceTime();
		ENCODER_SENS = MySettings::getScratchSens();

		#ifdef TINYUSB_NEED_POLLING_TASK
		// Manual call tud_task since it isn't called by Core's background
		TinyUSBDevice.task();
		#endif

		// not enumerated()/mounted() yet: nothing to do
		if (!TinyUSBDevice.mounted()) {
			return;
		}

		if (!usb_hid.ready()) return; // ?

		
		// ボタン処理
		for (int i = 0; i < 9; i++) {
			currentTime[i] = millis();
			currentRawState[i] = (digitalRead(keys[i]) == LOW);

			if (currentRawState[i] != button_isPressed[i]) {

				// デバウンスタイムが経過しているか確認
				if (currentTime[i] - button_lastChangeTime[i] > DEBOUNCE_DELAY_MS) {
					button_isPressed[i] = currentRawState[i];

					if (button_isPressed[i]) {
						gp.buttons |= (1U << i);
					}
					else {
						gp.buttons &= ~(1U << i);
					}
					button_lastChangeTime[i] = currentTime[i];
				}
			}
		}

		// ボタンのLED制御 デバウンス処理には含めない
		
		if (keyLedMode == 0) {

			// 消灯
			for (int i = 0; i < 9; i++) {
				digitalWrite(keyLeds[i], LOW);
			}

		} else if (keyLedMode == 1) {

			// 常時点灯
			for (int i = 0; i < 9; i++) {
				digitalWrite(keyLeds[i], HIGH);
			}

		} else if (keyLedMode == 2) {

			// 押下時点灯
			for (int i = 0; i < 9; i++) {
				if (digitalRead(keys[i]) == LOW) {
					digitalWrite(keyLeds[i], HIGH);
				} else {
					digitalWrite(keyLeds[i], LOW);
				}
			}

		} else if (keyLedMode == 3) {

			// flow
			keyLighting_flow();

		}

		
		
		// ロータリーエンコーダの処理
		int32_t encoder_count = quadrature_encoder_get_count(g_pio_enc_instance, g_pio_enc_sm);
		
		int32_t scaled_count = encoder_count / ENCODER_SENS;
  		uint8_t axis_value = (uint8_t)(scaled_count & 0xFF);
  		gp.x = axis_value;
		
		uint8_t scratchLedMode = MySettings::getScratchLedMode();
		if (scratchLedMode >= 40 && scratchLedMode <= 42 && !firstLoop) {

			int8_t diff = (int8_t)(axis_value - lastAxisValue);
			if (diff >= 1) {
				rp2040.fifo.push_nb(41);
			} else if (diff <= -1) {
				rp2040.fifo.push_nb(42);
			}
			lastAxisValue = axis_value;

		} else {
			lastAxisValue = axis_value;
			firstLoop = false;
		}


		

		// レポートの送信
		usb_hid.sendReport(0, &gp, sizeof(gp));

		settings_check_buttonState();

	}

	void keyLighting_flow() {

		for (int i = 7; i < 9; i++) {
			if (digitalRead(keys[i] == LOW)) {
				digitalWrite(keyLeds[i], HIGH);
			} else {
				digitalWrite(keyLeds[i], LOW);
			}
		}

		if (millis() - lastLedMoveTime < LED_MOVE_INTERVAL) {
			return;
		}

		lastLedMoveTime = millis();

		for (int i = 0; i < 7; i++) {
			digitalWrite(keyLeds[i], LOW);
		}


		digitalWrite(keyLeds[currLed], HIGH);

		if (currLed >= 6) {
			ledDirection = -1; // 反転
		} else if (currLed <= 0) {
			ledDirection = 1; // 正転
		}

		currLed = currLed + ledDirection;
  
	}

	uint8_t getAxisValue() {
	int32_t f_encoder_count = quadrature_encoder_get_count(g_pio_enc_instance, g_pio_enc_sm);
		
	int32_t f_scaled_count = f_encoder_count / 60;
  	uint8_t f_axis_value = (uint8_t)(f_scaled_count & 0xFF);
  		
		return f_axis_value;
	}
}


namespace {

	void settings_check_buttonState() {
		uint32_t now = millis();
		uint32_t elapsedTime = now - lastCheckTime;
		lastCheckTime = now;

		if (digitalRead(keys[7]) == LOW && digitalRead(keys[8]) == LOW && digitalRead(keys[0]) == LOW) {
			pressAccumulator += elapsedTime;
			if (pressAccumulator > LONG_PRESS_DURATION) {
				pressAccumulator = LONG_PRESS_DURATION;
			}
		} else {
			pressAccumulator -= elapsedTime;
			if (pressAccumulator < 0) {
			pressAccumulator = 0;
			}
		}

		if (pressAccumulator >= LONG_PRESS_DURATION && !longPressTriggered) {
			reset_gamepad_report();
			MySettings::changeMode();
			pressAccumulator = 0;
			longPressTriggered = true;
		}

		if (pressAccumulator == 0) {
			longPressTriggered = false;
		}
		
	}
	
	void reset_gamepad_report() {

		TinyUSBDevice.task();
		if (!usb_hid.ready()) return;

		gp.x = 0;
		gp.y = 0;
		gp.z = 0;
		gp.rz = 0;
		gp.rx = 0;
		gp.ry = 0;
		gp.hat = 0;
		gp.buttons = 0;
		usb_hid.sendReport(0, &gp, sizeof(gp));



		for (int i = 0; i < 9; i++) {
			digitalWrite(keyLeds[i], LOW);
		}
	}

	bool setupPioEncoder(uint pin_a) {
		g_pio_enc_sm = (int8_t)pio_claim_unused_sm(g_pio_enc_instance, true);
		if (g_pio_enc_sm == (uint)-1) {
			Serial.println("Error: PIOに空きSMがありません");
			return false;
		}

		uint offset = 0; 
		if (!pio_can_add_program_at_offset(g_pio_enc_instance, &quadrature_encoder_program, offset)) {
			Serial.println("Error: PIO 0番地にプログラムをロードできません");
			pio_sm_unclaim(g_pio_enc_instance, g_pio_enc_sm);
			return false;
		}
		pio_add_program_at_offset(g_pio_enc_instance, &quadrature_encoder_program, offset);

		int max_rate = 100000; // step数
		quadrature_encoder_program_init(g_pio_enc_instance, g_pio_enc_sm, pin_a, max_rate);

		return true;
	}

}
