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

#define LCD_WIDTH 84
#define LCD_HEIGHT 48

#define DISPLAY_MODE_BLANK 0
#define DISPLAY_MODE_ALL 1
#define DISPLAY_MODE_NORMAL 4
#define DISPLAY_MODE_INVERSE 5

extern const uint8_t SPACE[];
extern const uint8_t BANG[];
extern const uint8_t QUOTE[];
extern const uint8_t NUMBER[];
extern const uint8_t DOLLAR[];
extern const uint8_t PERCENT[];
extern const uint8_t AMP[];
extern const uint8_t APOSTROPHE[];
extern const uint8_t L_PAREN[];
extern const uint8_t R_PAREN[];
extern const uint8_t ASTERISK[];
extern const uint8_t PLUS[];
extern const uint8_t COMMA[];
extern const uint8_t HYPHEN[];
extern const uint8_t PERIOD[];
extern const uint8_t SLASH[];
extern const uint8_t ZERO[];
extern const uint8_t ONE[];
extern const uint8_t TWO[];
extern const uint8_t THREE[];
extern const uint8_t FOUR[];
extern const uint8_t FIVE[];
extern const uint8_t SIX[];
extern const uint8_t SEVEN[];
extern const uint8_t EIGHT[];
extern const uint8_t NINE[];
extern const uint8_t COLON[];
extern const uint8_t SEMICOLON[];
extern const uint8_t LESS_THAN[];
extern const uint8_t EQUALS[];
extern const uint8_t GREATER_THAN[];
extern const uint8_t QUESTION[];
extern const uint8_t AT[];
extern const uint8_t A_UP[];
extern const uint8_t B_UP[];
extern const uint8_t C_UP[];
extern const uint8_t D_UP[];
extern const uint8_t E_UP[];
extern const uint8_t F_UP[];
extern const uint8_t G_UP[];
extern const uint8_t H_UP[];
extern const uint8_t I_UP[];
extern const uint8_t J_UP[];
extern const uint8_t K_UP[];
extern const uint8_t L_UP[];
extern const uint8_t M_UP[];
extern const uint8_t N_UP[];
extern const uint8_t O_UP[];
extern const uint8_t P_UP[];
extern const uint8_t Q_UP[];
extern const uint8_t R_UP[];
extern const uint8_t S_UP[];
extern const uint8_t T_UP[];
extern const uint8_t U_UP[];
extern const uint8_t V_UP[];
extern const uint8_t W_UP[];
extern const uint8_t X_UP[];
extern const uint8_t Y_UP[];
extern const uint8_t Z_UP[];
extern const uint8_t L_BRACKET[];
extern const uint8_t BACKSLASH[];
extern const uint8_t R_BRACKET[];
extern const uint8_t CARET[];
extern const uint8_t UNDERSCORE[];
extern const uint8_t GRAVE[];
extern const uint8_t A_LO[];
extern const uint8_t B_LO[];
extern const uint8_t C_LO[];
extern const uint8_t D_LO[];
extern const uint8_t E_LO[];
extern const uint8_t F_LO[];
extern const uint8_t G_LO[];
extern const uint8_t H_LO[];
extern const uint8_t I_LO[];
extern const uint8_t J_LO[];
extern const uint8_t K_LO[];
extern const uint8_t L_LO[];
extern const uint8_t M_LO[];
extern const uint8_t N_LO[];
extern const uint8_t O_LO[];
extern const uint8_t P_LO[];
extern const uint8_t Q_LO[];
extern const uint8_t R_LO[];
extern const uint8_t S_LO[];
extern const uint8_t T_LO[];
extern const uint8_t U_LO[];
extern const uint8_t V_LO[];
extern const uint8_t W_LO[];
extern const uint8_t X_LO[];
extern const uint8_t Y_LO[];
extern const uint8_t Z_LO[];
extern const uint8_t L_BRACE[];
extern const uint8_t PIPE[];
extern const uint8_t R_BRACE[];
extern const uint8_t TILDE[];

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

/*
 * Clear the display and internal buffer.
 *
 * Returns false if the controller is not initialized, true otherwise.
 */
int clear();

/*
 * Draw a pixel to the specified x/y location. If state is true, the pixel is
 * turned on, and vice versa. The origin point is the top-left corner.
 *
 * Returns false if the controller is not initialized,
 * or x or y are out of range, true otherwise.
 */
int drawPixel(uint8_t x, uint8_t y, uint8_t state);

/*
 * Draw a region of the specified width and height to the screen. The passed x
 * and y coordinates are the location of the top-left corner of the region.
 * If opaque is true, bits that are off will clear the corresponding buffer
 * bits. If false, bits that are off have no effect.
 *
 * This function reads data in as columns, so the first byte corresponds to
 * coordinates (x,y) through (x,y+7), etc. When using an uneven column length, the
 * padding parameter will, if true, indicate that extra bits in the last byte
 * of a column are ignored, and if false, indicate the bits are the start of
 * the next column.
 *
 * Example: writing a 5x2 block with padding on would require 5 bytes.
 *   [11xxxxxx, 22xxxxxx, 33xxxxxx, 44xxxxxx, 55xxxxxx]
 * But the same size with padding off would use 2 bytes.
 *   [11223344, 55xxxxxx]
 *
 * Ultimately this function has better performance than its row counterpart.
 *
 * Returns false if the controller is not initialized, x or y are out of range,
 * or the height or width hand off the edge of the screen, true otherwise.
 */
int drawRegionColumns(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
        const uint8_t * buf, uint8_t padding, uint8_t opaque);

int drawRegionRows(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
        const uint8_t * buf, uint8_t padding, uint8_t opaque);

int drawText(uint8_t x, uint8_t y, const char * str, uint8_t opaque);

void love(void);
#endif
