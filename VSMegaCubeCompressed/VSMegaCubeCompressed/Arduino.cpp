#include "uncompress.h"
#ifndef _WIN32
#include <avr/pgmspace.h> // allows use of PROGMEM to store patterns in flash
#else
#include "arduino_util.h"
#include <iostream>
#endif

const unsigned long PROGMEM animation1[] = { 14UL,1UL,2299597343UL,2277226632UL,572662543UL,67108386UL,3722339940UL,4294862453UL,4017582087UL,1887997678UL,2685407095UL,428863485UL,235933184UL,3796046460UL,142344772UL,2760434432UL,476460205UL,327328272UL,4189589919UL,2264932200UL,1481035139UL,3682412UL,4056293500UL,1073749761UL,4148810488UL,3239458917UL,2113056UL,62915104UL,2185298498UL,3900982208UL,17825952UL,8651272UL };

int LEDPin[] = { A5, A4, 13, 12,
				 11, 10, 9, 8,
				 7, 6, 5, 4,
				 3, 2, 1, 0 };
int PlanePin[] = { A0, A1, A2, A3 };


void setup() {
	// DO NOT USE SERIAL BECAUSE PINS 0 AND 1 ARE USED FOR LEDS

	// Set up LED pins as output (active HIGH)
	for (int pin = 0; pin < PLANESIZE; pin++) {
		pinMode(LEDPin[pin], OUTPUT);
		digitalWrite(LEDPin[pin], LOW);
	}
	// Set up plane pins as outputs (active LOW)
	for (int pin = 0; pin < CUBESIZE; pin++) {
		pinMode(PlanePin[pin], OUTPUT);
		digitalWrite(PlanePin[pin], LOW);
	}
}
void loop() {
	unsigned long frames_count = getFrameCount(animation1);
	COMPRESSION compression_algo = getCompression(animation1);
	bool buffer_a[CUBESIZE * CUBESIZE * CUBESIZE];
	bool buffer_b[CUBESIZE * CUBESIZE * CUBESIZE];
	unsigned char metadata = 0;

	bool* previous_frame = buffer_a;
	bool* next_frame = buffer_b;

	// Loop over all frames of animation
	while (true) {
		unsigned long bits_offset = 0;

		for (unsigned long frame_id = 0; frame_id < frames_count; frame_id++) {
			bits_offset = readFrameDataAtOffset_P(animation1, bits_offset, next_frame, &metadata);

			if (frame_id != 0 && (compression_algo & COMPRESSION::FRAME_SUBTRACTION)) {
				// Reconstruct the next frame from the previous one
				addTwoFrames(previous_frame, next_frame);
			}

			unsigned long start_time = millis();
			unsigned long duration = (unsigned long)metadata * TIMECONST;
			while (millis() - start_time < duration) {
				showFrame(next_frame, metadata);
			}

			// Swap buffers
			bool* temp = previous_frame;
			previous_frame = next_frame;
			next_frame = temp;
		}
		break;
	}
}


void showFrame(bool* data, unsigned char metadata) {
	for (int plane = 0; plane < CUBESIZE; plane++) {
		// Turn previous plane off
		if (plane == 0) {
			digitalWrite(PlanePin[CUBESIZE - 1], LOW);
		} else {
			digitalWrite(PlanePin[plane - 1], LOW);
		}

		// Load current plane pattern data into ports
		int ledpin = 0;
		for (int ledrow = 0; ledrow < CUBESIZE; ledrow++) {
			for (int ledcol = 0; ledcol < CUBESIZE; ledcol++) {
				unsigned long index = (unsigned long)plane * CUBESIZE * CUBESIZE + ledrow * CUBESIZE + ledcol;
				digitalWrite(LEDPin[ledpin++], data[index] ? HIGH : LOW);
			}
		}

		// Turn current plane on
		digitalWrite(PlanePin[plane], HIGH);
		// Delay PLANETIME us
		delayMicroseconds(PLANETIME);
	}

	//String result = "";
	//for (int row = 0; row < CUBESIZE; row++) {
	//	for (int plane = 0; plane < CUBESIZE; plane++) {
	//		for (int col = 0; col < CUBESIZE; col++) {
	//			bool val = data[plane * CUBESIZE * CUBESIZE + row * CUBESIZE + col];
	//			result += val ? "1" : "0";
	//			result += " ";
	//		}
	//		result += "  ";
	//	}
	//	result += "\n";
	//}
	//Serial.println(result);
}
