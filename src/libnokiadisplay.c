#include "libnokiadisplay.h"
#include <util/delay.h>

inline void writeBit(volatile uint8_t * addr, uint8_t bit, uint8_t state) {
	if(state) *addr |= 1 << bit;
	else *addr &= ~(1 << bit);
}

int write(uint8_t byte, uint8_t dc) {
	return 0;
}

int initPorts(volatile uint8_t * resPort, uint8_t resBit,
		volatile uint8_t * enablePort, uint8_t enableBit,
		volatile uint8_t * dataPort, uint8_t dataBit,
		volatile uint8_t * clockPort, uint8_t clockBit) {
	if(resBit < 0 || resBit > 7 ||
			enableBit < 0 || enableBit > 7 ||
			dataBit < 0 || dataBit > 7 ||
			clockBit < 0 || clockBit > 7)
		return 1;

	while(1) {
		writeBit(resPort, resBit, 0);
		_delay_ms(500);
		writeBit(resPort, resBit, 1);
		_delay_ms(500);
	}
	return 0;
}

int initData(uint8_t bias, uint8_t vop, uint8_t tc) {
	return 0;
}

int setDisplayMode(uint8_t mode) {
	return 0;
}

int setPowerMode(uint8_t mode) {
	return 0;
}

int draw(uint8_t x, uint8_t y, uint8_t state) {
	return 0;
}

int love(void) {
	return 0;
}

