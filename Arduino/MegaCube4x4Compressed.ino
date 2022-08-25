#include <avr/pgmspace.h> // allows use of PROGMEM to store patterns in flash

#define CUBESIZE 4UL
#define PLANESIZE CUBESIZE*CUBESIZE
#define BLOCK_SIZE 4  // Compression block size
#define META_SIZE 5   // Metadata size
// Time duration of drawing a plane in one frame (us).
#define PLANETIME 2000
// Time unit for all frames (ms).
// Multiplied by the frame duration to give total time the frame is visible
#define TIMECONST 500 


void showFrame(bool* data, unsigned char metadata);
unsigned long getFrameCount(unsigned long* animation);
unsigned long readFrameDataAtOffset_P(unsigned long* animation, unsigned long bits_offset, bool* data, unsigned char* metadata);
unsigned long getFrameOffset_P(unsigned long* animation, unsigned long frame_id);
unsigned long readNumber_P(unsigned long* arr, unsigned long bits_offset, char bit_count);
void addTwoFrames(bool* frame1, bool* diff_frame);

const unsigned long PROGMEM animation1[] = { 4UL,1122976480UL,1122976495UL,547758111UL,547758246UL,2717917350UL,2719588889UL,2153357849UL,3175398724UL,3175398724UL,0UL };

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

      if (frame_id != 0) {
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
  //  for (int plane = 0; plane < CUBESIZE; plane++) {
  //    for (int col = 0; col < CUBESIZE; col++) {
  //      bool val = data[plane * CUBESIZE * CUBESIZE + row * CUBESIZE + col];
  //      result += val ? "1" : "0";
  //      result += " ";
  //    }
  //    result += "  ";
  //  }
  //  result += "\n";
  //}
  //Serial.println(result);
}


// Returns number of frames in animation
unsigned long getFrameCount(unsigned long* animation) {
  return animation[0];
}

// Reads binary data for frame starting at the binary offset.
// Returns bits offset for the next frame.
unsigned long readFrameDataAtOffset_P(unsigned long* animation, unsigned long bits_offset, bool* data, unsigned char* metadata) {
  unsigned long* start = animation + 1; // Jump over first number
  unsigned long frame_bits_count = 0;

  while (true) {
    unsigned char compressed_block_size;

    compressed_block_size = readNumber_P(start, bits_offset, BLOCK_SIZE);
    bits_offset += BLOCK_SIZE;

    if (compressed_block_size != 0) {
      // This block was compressed
      bool bit_value = readNumber_P(start, bits_offset++, 1);

      for (unsigned char i = 0; i < compressed_block_size; i++) {
        *(data++) = bit_value;
        frame_bits_count++;
      }
    } else {
      // This block was not compressed. Read raw data
      unsigned long uncompressed_block_size = CUBESIZE * CUBESIZE * CUBESIZE - frame_bits_count;

      for (unsigned long i = 0; i < uncompressed_block_size; i++) {
        *(data++) = readNumber_P(start, bits_offset++, 1);
        frame_bits_count++;
      }
    }

    if (frame_bits_count == CUBESIZE * CUBESIZE * CUBESIZE) {
      *metadata = readNumber_P(start, bits_offset, META_SIZE);
      bits_offset += META_SIZE;
      break;
    }
  }

  return bits_offset;
}

// Returns bits offset for the next frame.
unsigned long getFrameOffset_P(unsigned long* animation, unsigned long frame_id) {
  unsigned long* start = animation + 1; // Jump over first number
  unsigned long bits_offset = 0;
  unsigned long frame_bits_count = 0;
  unsigned long current_frame_id = 0;

  while (true) {
    unsigned char compressed_block_size;

    if (current_frame_id == frame_id) break;

    compressed_block_size = readNumber_P(start, bits_offset, BLOCK_SIZE);
    bits_offset += BLOCK_SIZE;

    if (compressed_block_size != 0) {
      // This block was compressed
      bool bit_value = readNumber_P(start, bits_offset++, 1);
      frame_bits_count += compressed_block_size;
    } else {
      // This block was not compressed. Read raw data
      unsigned long uncompressed_block_size = CUBESIZE * CUBESIZE * CUBESIZE - frame_bits_count;

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
unsigned long readNumber_P(unsigned long* arr, unsigned long bits_offset, char bit_count) {
  unsigned long dwords_offset = bits_offset / 32;
  unsigned long* start = arr + dwords_offset;     // Jumps over multiple of dwords
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
