/*
 * libnokiadisplay - A display library for Nokia 5110/3310 displays. Really
 * this works with the PCD8544 LCD controller (used by the Nokia displays),
 * so if other displays exist that use it, this library should still work (I
 * don't know if they do though). To use this library, the controller first
 * needs to be initialized as soon as possible. Then, extended register values
 * should be set. If you don't know, default values should work. Lastly, set
 * the display mode and power mode to activate the display.
 *
 * TODO: finish this
 */

#ifndef LIBNOKIADISPLAY_H
#define LIBNOKIADISPLAY_H

#include <stdint.h>

#define DISPLAY_MODE_BLANK 0
#define DISPLAY_MODE_ALL 1
#define DISPLAY_MODE_NORMAL 4
#define DISPLAY_MODE_INVERSE 5

/*
 * Reset the controller and set the appropriate signals to it. According to the
 * PCD8544 datasheet, this should be done as soon as possible. Other functions
 * will not work before this one is called. Parameters are port memory
 * addresses and respective bits in those addresses, for each pin connected to
 * the controller (such as address &PORTB, bit  6).
 * On Adafruit's Nokia breakout board, the corresponding controller pins are:
 * RST - resPort/Bit
 * CS - enablePort/Bit
 * D/C - selectorPort/Bit
 * DIN - dataPort/Bit
 * CLK - clockPort/Bit
 *
 * Returns false (and no reset action is performed) if any bits are out of
 * range (over 7), true otherwise.
 */
int initController(volatile uint8_t * resPort, uint8_t resBit,
		volatile uint8_t * enablePort, uint8_t enableBit,
		volatile uint8_t * selectorPort, uint8_t selectorBit,
		volatile uint8_t * dataPort, uint8_t dataBit,
		volatile uint8_t * clockPort, uint8_t clockBit);

/*
 * Set the bias system, operation voltage, and temperature coefficient
 * for driving the LCD.
 *
 * Returns false if the controller is not initialized
 * or parameters are out of range, true otherwise.
 */
int setExtendedRegisters(uint8_t bias, uint8_t vop, uint8_t tc);

/*
 * Set the bias system, operation voltage, and temperature coefficient
 * described above to default values of 4, 0x7f, and 0.
 *
 * Returns false if the controller is not initialized, true otherwise.
 */
int defaultSetExtendedRegisters();

/*
 * Set the display mode of the LCD. Possible values are:
 *   DISPLAY_MODE_BLANK: turn off all display segments
 *   DISPLAY_MODE_ALL: turn on all display segments
 *   DISPLAY_MODE_NORMAL: normal display from controller dram
 *   DISPLAY_MODE_INVERSE: inverse display from controller dram
 *
 * Returns false if the controller is not initialized
 * or an invalid mode is passed, true otherwise.
 */
int setDisplayMode(uint8_t mode);

/*
 * Set the power mode of the LCD. False sets power mode on (which matches the
 * PCD8544's function set command).
 *
 * Returns false if the controller is not initialized, true otherwise.
 */
int setPowerMode(uint8_t mode);

int drawPixel(uint8_t x, uint8_t y, uint8_t state);

void love(void);
#endif
