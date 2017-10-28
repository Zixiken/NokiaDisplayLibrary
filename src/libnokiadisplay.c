#include "libnokiadisplay.h"
#include <util/delay.h>

#define CMD_NORMAL 0x20
#define CMD_EXTENDED 0x21
#define CMD_VOP 0x80
#define CMD_TC 0x04
#define CMD_BIAS 0x10
#define CMD_DISPLAY_MODE 0x08
#define CMD_Y 0x40
#define CMD_X 0x80

#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define Y_HEIGHT 6
#define BUFFER_SIZE 504

volatile uint8_t * resP, * enableP, * dataP, * clockP, * selP;
uint8_t resM, enableM, dataM, clockM, selM;
uint8_t initialized = 0, powerMode = 4, x = 0, y = 0, vertical = 0;
uint8_t buffer[BUFFER_SIZE];

/*
 * Helper function to turn a bit on or off.
 */
static inline void writeBit(volatile uint8_t * addr, uint8_t mask, uint8_t state) {
	if(state) *addr |= mask;
	else *addr &= ~mask;
}

/*
 * Helper function for keeping track of the current coordinates.
 * Coordinates are incremented based on the current addressing mode.
 */
static inline void incrementCoordinates() {
	if(vertical && ++y == Y_HEIGHT) {
		y = 0;
		if(++x == LCD_WIDTH) x = 0;
	} else if(!vertical && ++x == LCD_WIDTH) {
		x = 0;
		if(++y == Y_HEIGHT) y = 0;
	}
}

/*
 * Send data or a command to the controller. If dc is true, the byte is
 * interpreted as data, and if false, as a command. The passed byte is sent
 * MSB-first, with minimal delay between clock edges.
 *
 * WARNING: Bit banging as fast as possible works fine on AVR microcontrollers
 * with 16Mhz or slower clock cycle, but other architectures may not delay
 * enough between clock edges. The PCD8544 requires a minimum of 100ns pulse
 * width, minimum of 250ns clock cycle.
 */
void send(uint8_t byte, uint8_t dc) {
	uint8_t mask;

	writeBit(selP, selM, dc);

	for(mask = 0x80; mask; mask >>= 1) {
		writeBit(dataP, dataM, byte & mask);
		*clockP |= clockM;
		*clockP &= ~clockM;
	}
}

/*
 * Helper function to send new X/Y coords to the controller if necessary
 */
static inline void setCoordinates(uint8_t newX, uint8_t newY) {
	if(x != newX) {
		x = newX;
		send(CMD_X | x, 0);
	}
	if(y != newY) {
		y = newY;
		send(CMD_Y | y, 0);
	}
}

int initController(volatile uint8_t * resPort, uint8_t resBit,
		volatile uint8_t * enablePort, uint8_t enableBit,
		volatile uint8_t * selectorPort, uint8_t selectorBit,
		volatile uint8_t * dataPort, uint8_t dataBit,
		volatile uint8_t * clockPort, uint8_t clockBit) {
	if(resBit > 7 || enableBit > 7 || dataBit > 7 || clockBit > 7 || selectorBit > 7)
		return 0;

	resM = 1 << resBit;
	*resPort &= resM;

	resP = resPort;
	enableP = enablePort;
	dataP = dataPort;
	clockP = clockPort;
	selP = selectorPort;

	enableM = 1 << enableBit;
	dataM = 1 << dataBit;
	clockM = 1 << clockBit;
	selM = 1 << selectorBit;

	*enableP |= enableM;
	*clockP &= clockM;

	*resP |= resM;

	initialized = 1;
	return 1;
}

int setExtendedRegisters(uint8_t bias, uint8_t vop, uint8_t tc) {
	if(!initialized || bias > 7 || vop > 0x7f || tc > 3) return 0;

	*enableP  &= ~enableM;
	send(CMD_EXTENDED | powerMode | vertical, 0);
	send(CMD_VOP | vop, 0);
	send(CMD_BIAS | bias, 0);
	send(CMD_TC | tc, 0);
	send(CMD_NORMAL | powerMode | vertical, 0);
	*enableP |= enableM;

	return 1;
}

int defaultSetExtendedRegisters() {
	if(!initialized) return 0;

	*enableP  &= ~enableM;
	send(CMD_EXTENDED | powerMode | vertical, 0);
	send(CMD_VOP | 0x7f, 0);
	send(CMD_BIAS | 4, 0);
	send(CMD_NORMAL | powerMode | vertical, 0);
	*enableP |= enableM;

	return 1;
}

int setDisplayMode(uint8_t mode) {
	if(!initialized) return 0;

	switch(mode) {
		case DISPLAY_MODE_BLANK:
		case DISPLAY_MODE_ALL:
		case DISPLAY_MODE_NORMAL:
		case DISPLAY_MODE_INVERSE:
			*enableP  &= ~enableM;
			send(CMD_DISPLAY_MODE | mode, 0);
			*enableP |= enableM;
			return 1;
		default:
			return 0;
	}
}

int setPowerMode(uint8_t mode) {
	if(!initialized) return 0;

	if(mode) powerMode = 4;
	else powerMode = 0;
	*enableP  &= ~enableM;
	send(CMD_NORMAL | powerMode | vertical, 0);
	*enableP |= enableM;

	return 1;
}

int clear() {
	if(!initialized) return 0;

	uint16_t i;
	*enableP &= ~enableM;
	for(i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0;
		send(0, 1);
	}
	*enableP |= enableM;

	return 1;
}

int drawPixel(uint8_t x, uint8_t y, uint8_t state) {
	if(!initialized || x >= LCD_WIDTH || y >= LCD_HEIGHT) return 0;

	uint8_t realY = y>>3, mask = 1<<(y&7);
	uint8_t * byte = buffer + realY*LCD_WIDTH + x;
	if(state) *byte |= mask;
	else *byte &= ~mask;

	*enableP &= ~enableM;
	setCoordinates(x, realY);
	send(*byte, 1);
	*enableP |= enableM;
	incrementCoordinates();

	return 1;
}

void love(void) {
}

