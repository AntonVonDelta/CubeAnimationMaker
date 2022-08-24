#include <avr/pgmspace.h> // allows use of PROGMEM to store patterns in flash

#define CUBESIZE 4UL
#define PLANESIZE CUBESIZE*CUBESIZE
#define PLANETIME 2000 // time each plane is displayed in us -> 100 Hz refresh
#define TIMECONST 50// multiplies DisplayTime to get ms - why not =100?
#define BLOCK_SIZE 4  // Compression block size
#define META_SIZE 5   // Metadata size


unsigned long getFrameCount(unsigned long* animation);
unsigned long readFrameDataAtOffset(unsigned long* animation, unsigned long bits_offset, bool* data, unsigned char* metadata);
unsigned long getFrameOffset(unsigned long* animation, unsigned long frame_id);
unsigned long readNumber(unsigned long* arr, unsigned long bits_offset, char bit_count);

unsigned long animation1[] = { 6UL,192953839UL,3249502877UL,2941493623UL,65685456UL,3220330474UL,2711756630UL,4294967293UL,791545771UL,57889UL,20UL };

int LEDPin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A4, A5 };
int PlanePin[] = { A0, A1, A2, A3 };

void setup() {
  int pin; // loop counter

  // set up LED pins as output (active HIGH)
  for (pin = 0; pin < PLANESIZE; pin++) {
    pinMode(LEDPin[pin], OUTPUT);
  }
  // set up plane pins as outputs (active LOW)
  for (pin = 0; pin < CUBESIZE; pin++) {
    pinMode(PlanePin[pin], OUTPUT);
  }
}

void loop() {
  unsigned long frames_count = getFrameCount(animation1);
  bool frame_data[CUBESIZE][CUBESIZE];

  // Loop over all frames of animation
  while (true) {
    for (unsigned long frame_id = 0; frame_id < frames_count; frame_id++) {


    }
  }
}

// Returns number of frames in animation
unsigned long getFrameCount(unsigned long* animation) {
  return animation[0];
}

// Reads binary data for frame starting at the binary offset.
// Returns bits offset for the next frame.
unsigned long readFrameDataAtOffset(unsigned long* animation, unsigned long bits_offset, bool* data, unsigned char* metadata) {
  unsigned long* start = animation + 1; // Jump over first number
  unsigned long frame_bits_count = 0;

  while (true) {
    unsigned char compressed_block_size;

    compressed_block_size = readNumber(start, bits_offset, BLOCK_SIZE);
    bits_offset += BLOCK_SIZE;

    if (compressed_block_size != 0) {
      // This block was compressed
      bool bit_value = readNumber(start, bits_offset++, 1);

      for (unsigned char i = 0; i < compressed_block_size; i++) {
        *(data++) = bit_value;
        frame_bits_count++;
      }
    }
    else {
      // This block was not compressed. Read raw data
      unsigned char uncompressed_block_size = CUBESIZE * CUBESIZE * CUBESIZE - frame_bits_count;

      for (unsigned long i = 0; i < uncompressed_block_size; i++) {
        *(data++) = readNumber(start, bits_offset++, 1);
        frame_bits_count++;
      }
    }

    if (frame_bits_count == CUBESIZE * CUBESIZE * CUBESIZE) {
      *metadata = readNumber(start, bits_offset, META_SIZE);
      bits_offset += META_SIZE;
      break;
    }
  }

  return bits_offset;
}

// Returns bits offset for the next frame.
unsigned long getFrameOffset(unsigned long* animation, unsigned long frame_id) {
  unsigned long* start = animation + 1; // Jump over first number
  unsigned long bits_offset = 0;
  unsigned long frame_bits_count = 0;
  unsigned long current_frame_id = 0;

  while (true) {
    unsigned char compressed_block_size;

    if (current_frame_id == frame_id) break;

    compressed_block_size = readNumber(start, bits_offset, BLOCK_SIZE);
    bits_offset += BLOCK_SIZE;

    if (compressed_block_size != 0) {
      // This block was compressed
      bool bit_value = readNumber(start, bits_offset++, 1);
      frame_bits_count += compressed_block_size;
    }
    else {
      // This block was not compressed. Read raw data
      unsigned char uncompressed_block_size = CUBESIZE * CUBESIZE * CUBESIZE - frame_bits_count;

      bits_offset += uncompressed_block_size;
      frame_bits_count += uncompressed_block_size;
    }

    if (frame_bits_count == CUBESIZE * CUBESIZE * CUBESIZE) {
      bits_offset += META_SIZE;
      current_frame_id++;
      frame_bits_count = 0;
    }
  }

  return bits_offset;
}

// Reads bit_count from arr after bits_offset and returns the number.
// Little endian
unsigned long readNumber(unsigned long* arr, unsigned long bits_offset, char bit_count) {
  unsigned long dwords_offset = bits_offset / 32;
  unsigned long* start = arr + dwords_offset;     // Jumps over multiple of dwords
  char remaining_offset_bits = bits_offset % 32;
  unsigned long result = 0;

  // Construct unsigned long starting at bits_offset
  if (remaining_offset_bits != 0) {
    unsigned long mask = (1UL << remaining_offset_bits) - 1;
    result = (*start) >> remaining_offset_bits;
    start += 1;

    result += ((*start) & mask) << (32 - remaining_offset_bits);
  }
  else  result = *start;

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
