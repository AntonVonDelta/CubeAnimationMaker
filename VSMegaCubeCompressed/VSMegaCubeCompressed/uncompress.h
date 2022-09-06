#pragma once
#ifdef _WIN32
#include "arduino_util.h"
#endif

#define CUBESIZE 4
#define PLANESIZE CUBESIZE*CUBESIZE
#define BLOCK_SIZE 4			// Size in bits of compression block size
#define META_SIZE 5				// Size in bits of frame meta
#define PLANETIME 2000			// Time duration of drawing a plane in one frame (us).
// Time unit for all frames (ms).
// Multiplied by the frame duration to give total time the frame is visible.
#define TIMECONST 10


enum COMPRESSION :unsigned long {
	NONE = 0,
	BLOCK = 1,
	FRAME_SUBTRACTION = 2
};

void showFrame(bool* data, unsigned char metadata);
unsigned long readFrameDataAtOffset_P(const unsigned long* animation, unsigned long bits_offset, bool* data, unsigned char* metadata);
unsigned long readNumber_P(const unsigned long* arr, unsigned long bits_offset, char bit_count);
void addTwoFrames(bool* frame1, bool* diff_frame);
unsigned long getFrameCount(const unsigned long* animation);
COMPRESSION getCompression(const unsigned long* animation);

void playAnimation(const unsigned long* animation, void (*funcShowFrame)(bool* data, unsigned char metadata)) {
	unsigned long frames_count = getFrameCount(animation);
	COMPRESSION compression_algo = getCompression(animation);
	bool buffer_a[CUBESIZE * CUBESIZE * CUBESIZE];
	bool buffer_b[CUBESIZE * CUBESIZE * CUBESIZE];
	unsigned char metadata = 0;

	bool* previous_frame = buffer_a;
	bool* next_frame = buffer_b;
	unsigned long bits_offset = 0;

	// Loop over all frames of animation
	for (unsigned long frame_id = 0; frame_id < frames_count; frame_id++) {
		bits_offset = readFrameDataAtOffset_P(animation, bits_offset, next_frame, &metadata);

		if (frame_id != 0 && (compression_algo & COMPRESSION::FRAME_SUBTRACTION)) {
			// Reconstruct the next frame from the previous one
			addTwoFrames(previous_frame, next_frame);
		}

		unsigned long start_time = millis();
		unsigned long duration = (unsigned long)metadata * TIMECONST;
		while (millis() - start_time < duration) {
			// Call provided callback to show the frame
			funcShowFrame(next_frame, metadata);
		}

		// Swap buffers
		bool* temp = previous_frame;
		previous_frame = next_frame;
		next_frame = temp;
	}
}

// Reads binary data for frame starting at the binary offset.
// Returns bits offset for the next frame.
unsigned long readFrameDataAtOffset_P(const unsigned long* animation, unsigned long bits_offset, bool* data, unsigned char* metadata) {
	const unsigned long* start = animation + 2; // Jump over frame count and metadata
	COMPRESSION compression_algo = getCompression(animation);
	unsigned long frame_bits_count = 0;

	if (!(compression_algo & COMPRESSION::BLOCK)) {
		for (unsigned long i = 0; i < CUBESIZE * CUBESIZE * CUBESIZE; i++) {
			*(data++) = readNumber_P(start, bits_offset++, 1);
		}
	} else {
		while (true) {
			unsigned long block_size;

			block_size = readNumber_P(start, bits_offset, BLOCK_SIZE);
			bits_offset += BLOCK_SIZE;

			if (block_size != 0) {
				// This block was compressed
				bool bit_value = readNumber_P(start, bits_offset++, 1);
				frame_bits_count += block_size;

				for (unsigned char i = 0; i < block_size; i++) {
					*(data++) = bit_value;
				}
			} else {
				// This block was not compressed. Read raw data
				block_size = CUBESIZE * CUBESIZE * CUBESIZE - frame_bits_count;
				frame_bits_count += block_size;

				for (unsigned long i = 0; i < block_size; i++) {
					*(data++) = readNumber_P(start, bits_offset++, 1);
				}
			}

			if (frame_bits_count == CUBESIZE * CUBESIZE * CUBESIZE) {
				break;
			}
		}
	}

	// Read frame metadata
	*metadata = readNumber_P(start, bits_offset, META_SIZE);
	bits_offset += META_SIZE;

	return bits_offset;
}

// Reads <bit_count> bits from array starting at <bits_offset> and returns the number.
// Little endian
unsigned long readNumber_P(const unsigned long* arr, unsigned long bits_offset, char bit_count) {
	unsigned long dwords_offset = bits_offset / 32;
	const unsigned long* start = arr + dwords_offset;     // Jumps over multiple of dwords
	char remaining_offset_bits = bits_offset % 32;
	unsigned long result = 0;

	// Construct unsigned long starting at bits_offset
	if (remaining_offset_bits != 0) {
		unsigned long mask = (1UL << remaining_offset_bits) - 1;
		result = pgm_read_dword(start) >> remaining_offset_bits;
		start += 1;

		result += (pgm_read_dword(start) & mask) << (32 - remaining_offset_bits);
	} else  result = pgm_read_dword(start);

	// Truncate at bit_count size only if not already 32bits
	if (bit_count == 32) return result;

	return result & ((1UL << bit_count) - 1);
}

// Adds to the parent/previous frame the difference to get the next frame.
// The result is saved in same memory as the difference.
void addTwoFrames(bool* frame1, bool* diff_frame) {
	for (unsigned long i = 0; i < CUBESIZE * CUBESIZE * CUBESIZE; i++) {
		diff_frame[i] = frame1[i] ^ diff_frame[i];
	}
}

// Returns number of frames in animation
unsigned long getFrameCount(const unsigned long* animation) {
	return pgm_read_dword(animation);
}
COMPRESSION getCompression(const unsigned long* animation) {
	return (COMPRESSION)pgm_read_dword(animation + 1);
}