#pragma once
#ifdef _WIN32
#include "arduino_util.h"
#endif

#define CUBESIZE 4UL
#define PLANESIZE CUBESIZE*CUBESIZE
// Size in bits of compression block size
#define BLOCK_SIZE 4  
// Size in bits of frame meta
#define META_SIZE 5
// Time duration of drawing a plane in one frame (us).
#define PLANETIME 2000
// Time unit for all frames (ms).
// Multiplied by the frame duration to give total time the frame is visible
#define TIMECONST 1000

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

		*metadata = readNumber_P(start, bits_offset, META_SIZE);
		bits_offset += META_SIZE;
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
				*metadata = readNumber_P(start, bits_offset, META_SIZE);
				bits_offset += META_SIZE;
				break;
			}
		}
	}

	return bits_offset;
}
// Reads bit_count from arr after bits_offset and returns the number.
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