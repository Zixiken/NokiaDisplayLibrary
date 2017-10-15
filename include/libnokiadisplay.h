#ifndef LIBNOKIADISPLAY_H
#define LIBNOKIADISPLAY_H

#include <stdint.h>

int initPorts(volatile uint8_t * resPort, uint8_t resBit,
		volatile uint8_t * enablePort, uint8_t enableBit,
		volatile uint8_t * dataPort, uint8_t dataBit,
		volatile uint8_t * clockPort, uint8_t clockBit);
int initData(uint8_t bias, uint8_t vop, uint8_t tc);
int setDisplayMode(uint8_t mode);
int setPowerMode(uint8_t mode);
int draw(uint8_t x, uint8_t y, uint8_t state);
int love(void);
#endif
