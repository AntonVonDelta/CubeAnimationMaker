#pragma once
#include <string>
#include <iostream>

#define A0 100
#define A1 101
#define A2 102
#define A3 103
#define A4 104
#define A5 105

#define pinMode(a,b);
#define pgm_read_dword(address_short) *address_short
#define digitalWrite(a,b);
#define delayMicroseconds(a);
#define millis() 1
#define PROGMEM

typedef std::string String;

class SerialClass {
public:

	void begin(int baud) {
	}

	void println(unsigned long nr) {
		std::cout << nr << std::endl;
	}

	void println(std::string str) {
		std::cout << str << std::endl;
	}

} Serial;