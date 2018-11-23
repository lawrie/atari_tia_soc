#include <stdint.h>
#include <stdbool.h>
#include <uart/uart.h>
#include <ili9341/ili9341.h>
#include <delay/delay.h>
#include <button/button.h>

#define reg_uart_clkdiv (*(volatile uint32_t*)0x02000004)
#define reg_buttons (*(volatile uint32_t*)0x03000004)
#define reg_uart_data (*(volatile uint32_t*)0x02000008)

#define reg_pf (*(volatile uint32_t*)0x05000010)
#define reg_color (*(volatile uint32_t*)0x05000014)
#define reg_room (*(volatile uint32_t*)0x05000018)
#define reg_obj (*(volatile uint32_t*)0x0500001C)

#define Y_OFFSET 24

#define DARK_GREEN 0
#define BROWN 1
#define LIGHT_GREEN 2
#define YELLOW 3
#define YELLOW_GREEN 4
#define BLUE 5
#define BLACK 6
#define WHITE 7
#define RED 8
#define PINK 9
#define GREY 10
#define PURPLE 11
#define GOLD 12

// Colours
const uint16_t colors[] = {
                     0x22A2, // Dark green
                     0x6142, // Brown
                     0x56A7, // Light green
                     0xAD46, // Light Yellow
                     0x6B60, // Dark Yellow
                     0x3655, // Light blue
                     0x0000, // Black
                     0xFFFF, // White
                     0xF100, // Red
                     0xD596, // Pink
                     0xBDB6, // Grey
                     0xB216, // Purple
                     0xFe00, // Gold
};

#define HARRY_MIN 8
#define HARRY_MAX 148

uint8_t harry0[] = {
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00110011, // |  XX  XX|
    0b01110010, // | XXX  X |
    0b11011010, // |XX XX X |
    0b00011110, // |   XXXX |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b01011000, // | X XX   |
    0b01011000, // | X XX   |
    0b01111100, // | XXXXX  |
    0b00111110, // |  XXXXX |
    0b00011010, // |   XX X |
    0b00011000, // |   XX   |
    0b00010000, // |   X    |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t harry1[] = {
    0b00000000, // |        |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b11000011, // |XX    XX|
    0b01100010, // | XX   X |
    0b01100010, // | XX   X |
    0b00110110, // |  XX XX |
    0b00111110, // |  XXXXX |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00111100, // |  XXXX  |
    0b00111110, // |  XXXXX |
    0b00111010, // |  XXX X |
    0b00111000, // |  XXX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00010000, // |   X    |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t harry2[] = {
    0b00010000, // |   X    |
    0b00100000, // |  X     |
    0b00100010, // |  X   X |
    0b00100100, // |  X  X  |
    0b00110100, // |  XX X  |
    0b00110010, // |  XX  X |
    0b00010110, // |   X XX |
    0b00011110, // |   XXXX |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011100, // |   XXX  |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00010000, // |   X    |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t harry3[] = {
    0b00001100, // |    XX  |
    0b00001000, // |    X   |
    0b00101000, // |  X X   |
    0b00101000, // |  X X   |
    0b00111110, // |  XXXXX |
    0b00001010, // |    X X |
    0b00001110, // |    XXX |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011100, // |   XXX  |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00010000, // |   X    |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t harry4[] = {
    0b00000000, // |        |
    0b00000010, // |      X |
    0b01000011, // | X    XX|
    0b01000100, // | X   X  |
    0b01110100, // | XXX X  |
    0b00010100, // |   X X  |
    0b00011100, // |   XXX  |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00111100, // |  XXXX  |
    0b00111110, // |  XXXXX |
    0b00111010, // |  XXX X |
    0b00111000, // |  XXX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00010000, // |   X    |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t harry5[] = {
    0b00011000, // |   XX   |
    0b00010000, // |   X    |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011100, // |   XXX  |
    0b00011110, // |   XXXX |
    0b00011010, // |   XX X |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00010000, // |   X    |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t harry6[] = {
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b01100011, // | XX   XX|
    0b11110010, // |XXXX  X |
    0b11110110, // |XXXX XX |
    0b11011100, // |XX XXX  |
    0b11000000, // |XX      |
    0b11000000, // |XX      |
    0b11000000, // |XX      |
    0b11000000, // |XX      |
    0b11000000, // |XX      |
    0b11110000, // |XXXX    |
    0b11010000, // |XX X    |
    0b10010000, // |X  X    |
    0b11010000, // |XX X    |
    0b11010000, // |XX X    |
    0b11000000, // |XX      |
    0b00000000, // |        |
};

uint8_t harry7[] = {
    0b00110000, // |  XX    |
    0b00010000, // |   X    |
    0b00010000, // |   X    |
    0b00010000, // |   X    |
    0b00010110, // |   X XX |
    0b00010100, // |   X X  |
    0b00010100, // |   X X  |
    0b00010110, // |   X XX |
    0b00010010, // |   X  X |
    0b00010110, // |   X XX |
    0b00011110, // |   XXXX |
    0b00011100, // |   XXX  |
    0b00011000, // |   XX   |
    0b00111000, // |  XXX   |
    0b00111000, // |  XXX   |
    0b00111100, // |  XXXX  |
    0b00011110, // |   XXXX |
    0b00011010, // |   XX X |
    0b00000010, // |      X |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
};

uint8_t harry8[] = {
    0b00001100, // |    XX  |
    0b00001000, // |    X   |
    0b00001000, // |    X   |
    0b00001000, // |    X   |
    0b01101000, // | XX X   |
    0b00101000, // |  X X   |
    0b00101000, // |  X X   |
    0b01101000, // | XX X   |
    0b01001000, // | X  X   |
    0b01101000, // | XX X   |
    0b01111000, // | XXXX   |
    0b00111000, // |  XXX   |
    0b00011000, // |   XX   |
    0b00011100, // |   XXX  |
    0b00011100, // |   XXX  |
    0b00111100, // |  XXXX  |
    0b01111000, // | XXXX   |
    0b01011000, // | X XX   |
    0b01000000, // | X      |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
};

uint8_t *harry[] = {harry0, harry1, harry2, harry3, harry4, harry5, harry6, harry7, harry8};

uint8_t log1[] = {
    0b00000000, // |        |
    0b00011000, // |   XX   |
    0b00100100, // |  X  X  |
    0b01011010, // | X XX X |
    0b01011010, // | X XX X |
    0b01011010, // | X XX X |
    0b01100110, // | XX  XX |
    0b01111110, // | XXXXXX |
    0b01011110, // | X XXXX |
    0b01110110, // | XXX XX |
    0b01111110, // | XXXXXX |
    0b01011110, // | X XXXX |
    0b01110110, // | XXX XX |
    0b00111100, // |  XXXX  |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t log2[] = {
    0b00000000, // |        |
    0b00011000, // |   XX   |
    0b00100100, // |  X  X  |
    0b01011010, // | X XX X |
    0b01011010, // | X XX X |
    0b01011010, // | X XX X |
    0b01100110, // | XX  XX |
    0b01111110, // | XXXXXX |
    0b01111010, // | XXXX X |
    0b01101110, // | XX XXX |
    0b01111110, // | XXXXXX |
    0b01111010, // | XXXX X |
    0b01101110, // | XX XXX |
    0b00111100, // |  XXXX  |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t fire0[] = {
    0b00000000, // |        |
    0b11000011, // |XX    XX|
    0b11100111, // |XXX  XXX|
    0b01111110, // | XXXXXX |
    0b00111100, // |  XXXX  |
    0b00011000, // |   XX   |
    0b00111100, // |  XXXX  |
    0b01111100, // | XXXXX  |
    0b01111100, // | XXXXX  |
    0b01111000, // | XXXX   |
    0b00111000, // |  XXX   |
    0b00111000, // |  XXX   |
    0b00110000, // |  XX    |
    0b00110000, // |  XX    |
    0b00010000, // |   X    |
    0b00010000, // |   X    |
};

uint8_t fire1[] = {
    0b00000000, // |        |
    0b11000011, // |XX    XX|
    0b11100111, // |XXX  XXX|
    0b01111110, // | XXXXXX |
    0b00111100, // |  XXXX  |
    0b00011000, // |   XX   |
    0b00111100, // |  XXXX  |
    0b00111110, // |  XXXXX |
    0b00111110, // |  XXXXX |
    0b00011110, // |   XXXX |
    0b00011100, // |   XXX  |
    0b00011100, // |   XXX  |
    0b00001100, // |    XX  |
    0b00001100, // |    XX  |
    0b00001000, // |    X   |
    0b00001000, // |    X   |
};

uint8_t cobra0[] = {
    0b00000000, // |        |
    0b11111110, // |XXXXXXX |
    0b11111001, // |XXXXX  X|
    0b11111001, // |XXXXX  X|
    0b11111001, // |XXXXX  X|
    0b11111001, // |XXXXX  X|
    0b01100000, // | XX     |
    0b00010000, // |   X    |
    0b00001000, // |    X   |
    0b00001100, // |    XX  |
    0b00001100, // |    XX  |
    0b00001000, // |    X   |
    0b00111000, // |  XXX   |
    0b00110000, // |  XX    |
    0b01000000, // | X      |
    0b00000000, // |        |
};

uint8_t cobra1[] = {
    0b00000000, // |        |
    0b11111110, // |XXXXXXX |
    0b11111001, // |XXXXX  X|
    0b11111001, // |XXXXX  X|
    0b11111010, // |XXXXX X |
    0b11111010, // |XXXXX X |
    0b01100000, // | XX     |
    0b00010000, // |   X    |
    0b00001000, // |    X   |
    0b00001100, // |    XX  |
    0b00001100, // |    XX  |
    0b00001000, // |    X   |
    0b00111000, // |  XXX   |
    0b00110000, // |  XX    |
    0b10000000, // |X       |
    0b00000000, // |        |
};

uint8_t croc0[] = {
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b11111111, // |XXXXXXXX|
    0b10101011, // |X X X XX|
    0b00000011, // |      XX|
    0b00000011, // |      XX|
    0b00001011, // |    X XX|
    0b00101110, // |  X XXX |
    0b10111010, // |X XXX X |
    0b11100000, // |XXX     |
    0b10000000, // |X       |
    0b00000000, // |        |
    0b00000000, // |        |
};

uint8_t croc1[] = {
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b11111111, // |XXXXXXXX|
    0b10101011, // |X X X XX|
    0b01010101, // | X X X X|
    0b11111111, // |XXXXXXXX|
    0b00000110, // |     XX |
    0b00000100, // |     X  |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
};

uint8_t money_bag[] = {
    0b00000000, // |        |
    0b00111110, // |  XXXXX |
    0b01110111, // | XXX XXX|
    0b01110111, // | XXX XXX|
    0b01100011, // | XX   XX|
    0b01111011, // | XXXX XX|
    0b01100011, // | XX   XX|
    0b01101111, // | XX XXXX|
    0b01100011, // | XX   XX|
    0b00110110, // |  XX XX |
    0b00110110, // |  XX XX |
    0b00011100, // |   XXX  |
    0b00001000, // |    X   |
    0b00011100, // |   XXX  |
    0b00110110, // |  XX XX |
    0b00000000, // |        |
};

uint8_t scorpion0[] = {
    0b10000101, // |X    X X|
    0b00110010, // |  XX  X |
    0b00111101, // |  XXXX X|
    0b01111000, // | XXXX   |
    0b11111000, // |XXXXX   |
    0b11000110, // |XX   XX |
    0b10000010, // |X     X |
    0b10010000, // |X  X    |
    0b10001000, // |X   X   |
    0b11011000, // |XX XX   |
    0b01110000, // | XXX    |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
};

uint8_t scorpion1[] = {
    0b01001001, // | X  X  X|
    0b00110011, // |  XX  XX|
    0b00111100, // |  XXXX  |
    0b01111000, // | XXXX   |
    0b11111010, // |XXXXX X |
    0b11000100, // |XX   X  |
    0b10010010, // |X  X  X |
    0b10001000, // |X   X   |
    0b11011000, // |XX XX   |
    0b01110000, // | XXX    |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
};

uint8_t wall[] = {
    0b11111110, // |XXXXXXX |
    0b10111010, // |X XXX X |
    0b10111010, // |X XXX X |
    0b10111010, // |X XXX X |
    0b11111110, // |XXXXXXX |
    0b11101110, // |XXX XXX |
    0b11101110, // |XXX XXX |
    0b11101110, // |XXX XXX |
    0b11111110, // |XXXXXXX |
    0b10111010, // |X XXX X |
    0b10111010, // |X XXX X |
    0b10111010, // |X XXX X |
    0b11111110, // |XXXXXXX |
    0b11101110, // |XXX XXX |
    0b11101110, // |XXX XXX |
    0b11101110, // |XXX XXX |
};

uint8_t bar0[] = {
    0b00000000, // |        |
    0b11111000, // |XXXXX   |
    0b11111100, // |XXXXXX  |
    0b11111110, // |XXXXXXX |
    0b11111110, // |XXXXXXX |
    0b01111110, // | XXXXXX |
    0b00111110, // |  XXXXX |
    0b00000000, // |        |
    0b00010000, // |   X    |
    0b00000000, // |        |
    0b01010100, // | X X X  |
    0b00000000, // |        |
    0b10010010, // |X  X  X |
    0b00000000, // |        |
    0b00010000, // |   X    |
    0b00000000, // |        |
};

uint8_t bar1[] = {
    0b00000000, // |        |
    0b11111000, // |XXXXX   |
    0b11111100, // |XXXXXX  |
    0b11111110, // |XXXXXXX |
    0b11111110, // |XXXXXXX |
    0b01111110, // | XXXXXX |
    0b00111110, // |  XXXXX |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00101000, // |  X X   |
    0b00000000, // |        |
    0b01010100, // | X X X  |
    0b00000000, // |        |
    0b00010000, // |   X    |
    0b00000000, // |        |
    0b00000000, // |        |
};

uint8_t ring[] = {
    0b00000000, // |        |
    0b00000000, // |        |
    0b00111000, // |  XXX   |
    0b01101100, // | XX XX  |
    0b01000100, // | X   X  |
    0b01000100, // | X   X  |
    0b01000100, // | X   X  |
    0b01101100, // | XX XX  |
    0b00111000, // |  XXX   |
    0b00010000, // |   X    |
    0b00111000, // |  XXX   |
    0b01111100, // | XXXXX  |
    0b00111000, // |  XXX   |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
};

uint8_t nothing[] = {
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
};

uint8_t zero[] = {
    0b00111100, // |  XXXX  |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b00111100, // |  XXXX  |
};

uint8_t one[] = {
    0b00111100, // |  XXXX  |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00111000, // |  XXX   |
    0b00011000, // |   XX   |
};

uint8_t two[] = {
    0b01111110, // | XXXXXX |
    0b01100000, // | XX     |
    0b01100000, // | XX     |
    0b00111100, // |  XXXX  |
    0b00000110, // |     XX |
    0b00000110, // |     XX |
    0b01000110, // | X   XX |
    0b00111100, // |  XXXX  |
};

uint8_t three[] = {
    0b00111100, // |  XXXX  |
    0b01000110, // | X   XX |
    0b00000110, // |     XX |
    0b00001100, // |    XX  |
    0b00001100, // |    XX  |
    0b00000110, // |     XX |
    0b01000110, // | X   XX |
    0b00111100, // |  XXXX  |
};

uint8_t four[] = {
    0b00001100, // |    XX  |
    0b00001100, // |    XX  |
    0b00001100, // |    XX  |
    0b01111110, // | XXXXXX |
    0b01001100, // | X  XX  |
    0b00101100, // |  X XX  |
    0b00011100, // |   XXX  |
    0b00001100, // |    XX  |
};

uint8_t five[] = {
    0b01111100, // | XXXXX  |
    0b01000110, // | X   XX |
    0b00000110, // |     XX |
    0b00000110, // |     XX |
    0b01111100, // | XXXXX  |
    0b01100000, // | XX     |
    0b01100000, // | XX     |
    0b01111110, // | XXXXXX |
};

uint8_t six[] = {
    0b00111100, // |  XXXX  |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b01111100, // | XXXXX  |
    0b01100000, // | XX     |
    0b01100010, // | XX   X |
    0b00111100, // |  XXXX  |
};

uint8_t seven[] = {
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00001100, // |    XX  |
    0b00000110, // |     XX |
    0b01000010, // | X    X |
    0b01111110, // | XXXXXX |
};

uint8_t eight[] = {
    0b00111100, // |  XXXX  |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b00111100, // |  XXXX  |
    0b00111100, // |  XXXX  |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b00111100, // |  XXXX  |
};

uint8_t nine[] = {
    0b00111100, // |  XXXX  |
    0b01000110, // | X   XX |
    0b00000110, // |     XX |
    0b00111110, // |  XXXXX |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b01100110, // | XX  XX |
    0b00111100, // |  XXXX  |
};

uint8_t colon[] = {
    0b00000000, // |        |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00011000, // |   XX   |
    0b00011000, // |   XX   |
    0b00000000, // |        |
};

uint8_t space[] = {
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
    0b00000000, // |        |
};

#ifdef copyright
uint8_t f[] = {
    0b11111100, // |XXXXXX  |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b11111100, // |XXXXXX  |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b10000000, // |X       |
};

uint8_t p[] = {
    0b11111000, // |XXXXX   |
    0b10000100, // |X    X  |
    0b10000100, // |X    X  |
    0b10000100, // |X    X  |
    0b11111000, // |XXXXX   |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b10000000, // |X       |
};

uint8_t g[] = {
    0b01111000, // | XXXX   |
    0b10000100, // |X    X  |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b10111000, // |X XXX   |
    0b10000100, // |X    X  |
    0b10000100, // |X    X  |
    0b01111000, // | XXXX   |
};

uint8_t c[] = {
    0b01111000, // | XXXX   |
    0b10000100, // |X    X  |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b10000000, // |X       |
    0b10000100, // |X    X  |
    0b01111000, // | XXXX   |
};
#endif 

uint32_t leaves[][4] = {
    {0xfffff, 0x3cff3, 0x183c1, 0x00180},
    {0xe7ffe, 0xc3dbc, 0x81818, 0x00000},
    {0xfffff, 0xffe7f, 0xdbc3d, 0x81818},
    {0xffe7f, 0xffc3f, 0xc3c3c, 0xc300c}
};

uint32_t trees[] = {0x08080, 0x02020, 0x01008, 0x00404};

uint32_t pool[] = {
    0x0000F,
    0x0003F,
    0x000FF,
    0x003FF,
    0x000FF,
    0x0003F,
    0x0000F
};

uint32_t ground_pf[7];
uint16_t pool_color;

uint8_t *numbers[] = {zero, one, two, three, four, five, six, seven, eight, nine};

#define STOPPED 0
#define FORWARDS 1
#define BACKWARDS 2

#define HOLE1_SCENE 0
#define HOLE3_SCENE 1
#define CROCO_SCENE 4
#define TREASURE_SCENE 5

#define ID_STATIONARY 4
#define ID_FIRE 6
#define ID_COBRA 7
#define ID_TREASURES 8
#define ID_NOTHING 12

#define ID_KNEEING  0
#define ID_RUNNING4 4

#define ID_SWINGING 6
#define ID_CLIMBING 7

uint8_t scene, scene_type, object_type, tree_pat, harry_x, harry_dir;
bool underground;

#ifdef get_pixel
// Get 20-bit wall data for given row
uint32_t get_background(uint8_t y) {
  if (y < 64) return 0x00000;
  else if (y < 112) return 0x01010;
  else return 0x00000;
}
 
// Get pixel corresponding to room co-ordinates 
bool get_pixel(uint8_t r, uint8_t x, uint8_t y) {
  uint32_t row = get_background(y);
  uint32_t mask = 1 << ((x >=80 ? x-80 : 79-x) >> 2);
  return (row & mask);
}
#endif

// Start write a block of pixels
void start_write(int y) {
  lcd_set_window(0, Y_OFFSET+y, WIDTH-1, HEIGHT-1);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;
}

// Draw a horizontal stripe of colour
void draw_stripe(int y, uint16_t col, int n) {
  start_write(y);
  reg_pf = 0;
  reg_color = col << 16;
  reg_room = n;
}

// Draw the top of the screen
void draw_top() {
  draw_stripe(0, colors[DARK_GREEN], 15360);
}

// draw the leaves
void draw_leaves() {
  start_write(48);
  reg_color = (colors[LIGHT_GREEN] << 16) | colors[DARK_GREEN];

  for(int i=0;i<4;i++) {
    reg_pf = leaves[tree_pat][i];
    reg_room = 1280;
  }
}

// draw the trees
void draw_trees() {
  start_write(64);
  reg_pf = trees[tree_pat];
  reg_color = (colors[LIGHT_GREEN] << 16) | colors[BROWN];
  reg_room = 15360;
}

// draw the ground
void draw_ground() {
  start_write(112);
  reg_color = (colors[YELLOW] << 16) | pool_color;

  reg_pf = 0;
  reg_room = 320;

  for(int i=0;i<7;i++) {
    reg_pf = ground_pf[i];
    reg_room = 640;
  }

  reg_pf = 0;
  reg_room = 320;
}

// Draw the lower ground
void draw_lower_ground() {
  start_write(128);
  reg_color = colors[YELLOW_GREEN];
  reg_pf = (scene_type == 1 ? 0xFFF1E : (scene_type == 0 ? 0xFFFFE : 0xFFFFF));
  reg_room = 5120;
}

// draw the underground
void draw_underground() {
   draw_stripe(176, colors[YELLOW_GREEN], 2560);
}

void draw_object(uint8_t x0, uint8_t y0, uint16_t oc, uint16_t bc, 
                      const uint8_t *d, bool flip) {
  lcd_set_window(x0 << 1, Y_OFFSET + y0, (x0 << 1) + 15, Y_OFFSET + y0 + 15);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;
  reg_color = (bc << 16) |  oc;

  for(int y=0;y<16;y++) {
    uint8_t c = *(d + 15);
      
    reg_obj = c | (flip ? 0x100 : 0);

    d--;
  }
}

// Draw object with two colours
void draw_object_2col(uint8_t x, uint8_t y, uint16_t oc1, uint16_t oc2,
                 const uint8_t *d) {
  for(int n=0;n<16;n++) {
    uint8_t c = *(d + 15);
    for(int i=0;i<16;i++) {
      for(int j=0;(c & 0x80) && j<2;j++) 
        lcd_draw_pixel(((x + i) <<1) + j, Y_OFFSET + y, n < 8 ? oc1 : oc2);
      c <<= 1;
    }
    d--;
    y++;
  }
}

// Simple multiply
int mult(int x, int y) {
  int res = 0;

  for(int i=0;i<x;i++) res += y;

  return res;
}

// draw of undraw a block of pixels
void draw_block(int x, int y, int w, int h, uint16_t col) {
  lcd_set_window((x << 1), Y_OFFSET+y, (x << 1) + w - 1, Y_OFFSET + y + h - 1);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;
  reg_fast_xfer = (mult(w, h) << 16) | col;
}

// Draw Harry
void draw_harry(uint8_t x, uint8_t y, const uint8_t *d, bool flip) {
  for(int n=0;n<22;n++) {
    int col = (n < 2 ? 1 : (n < 6 ? 9 : (n < 12 ? 5 : 0)));
    uint8_t c = *(d + 21);
    for(int i=0;i<16;i++) {
      for(int j=0;(c & (flip ? 1 : 0x80)) && j<2;j++) 
        lcd_draw_pixel(((x + i) <<1) + j, Y_OFFSET + y, colors[col]);
      if (flip) c >>= 1; else c <<= 1;
    }
    d--;
    y++;
  }
}

// Undraw Harry
void undraw_harry(uint8_t x, uint8_t y, const uint8_t *d, bool flip) {
  if (!underground) draw_trees();
  else draw_block(x, y, 16, 22, 0);
}

// Draw number or letter
void draw_number(uint8_t x, uint8_t y, uint16_t oc, 
                 const uint8_t *d, bool flip) {
  lcd_set_window(x << 1, Y_OFFSET + y, (x << 1) + 15, Y_OFFSET + y + 7);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;
  reg_color = (colors[DARK_GREEN] << 16) | oc;

  for(int n=0;n<8;n++) {
    uint8_t c = *(d + (flip ? 7 : 0));

    reg_obj = c;

    d += (flip ? -1 : 1);
    y++;
  }
}

#ifdef copyright
void draw_copyright() {
  draw_number(40, 194, colors[WHITE], f, false);
  draw_number(50, 194, colors[WHITE], p, false);
  draw_number(60, 194, colors[WHITE], g, false);
  draw_number(70, 194, colors[WHITE], c, false);
}
#endif 

const int divisor[] = {10000,1000,100,10};

// Display score, hi-score or another numnber
void show_score(int x, int y, int score, uint16_t col) {
  int s = score;
  bool blank = true;
  for(int i=0; i<5; i++) {
    int d = 0;
    if (i == 4) d = s;
    else {
      int div = divisor[i];
      while (s >= div) {
        s -= div;
        d++;
      }
      if (d !=0) blank = false;
    }
    if (!blank || i == 4)
      draw_number(x + (i*10), y, col, numbers[d], true); 
  }
}

// Display two-digit number
void show_two_digits(int x, int y, int n, uint16_t col) {
  int s = n;
  for(int i=0; i<2; i++) {
    int d = 0;
    if (i == 1) d = s;
    else {
      int div = divisor[i+3];
      while (s >= div) {
        s -= div;
        d++;
      }
    }
    draw_number(x + (i*10), y, col, numbers[d], true); 
  }
}

// draw remaining time
void draw_time(int x, int y, int min, int sec, uint16_t col) {
  show_two_digits(x, y, min, col);
  draw_number(x + 20, y, col, colon, true);
  show_two_digits(x + 30, y, sec, col);
}

// Show the remaining lives
void show_lives(int x, int y, int lives) {
  for(int i=0;i<lives;i++) 
    draw_block(x + i*4, y, 2, 8, colors[WHITE]);
}

// draw the ladder
void draw_ladder(int x, bool undraw) {
  for(int n=0;n<11;n++) {
    draw_block(x+2, 132 + 4*n, 8, 2, undraw ? colors[BLACK] : colors[YELLOW_GREEN]);
  }
}

// draw a wall
void draw_wall(int x, bool undraw) {
  draw_object(x, 144, colors[undraw ? BLACK : RED], 0, wall, false);
  draw_object(x, 160, colors[undraw ? BLACK : RED], 0, wall, false);

  for(int n=0;n<8;n++) 
    for(int i=0;i<15;i++)
      draw_block(x, 147 + n*4, 15, 1, undraw ? colors[BLACK] : colors[GREY]); 
}

// draw the liana
void draw_vine() {
  for(int y=48; y<96; y++) 
   for(int x=159; x<161;x++) 
     lcd_draw_pixel(x, Y_OFFSET+y,colors[BROWN]);
}  

// Set variables for new scene
void set_scene() {
  scene_type = (scene >> 3) & 7;
  tree_pat = (scene >> 6) & 3;
  object_type = scene & 7;
};

// Move one screen to the right
void right_scene() {
  scene = (scene << 1) | (((scene >> 3) ^ (scene >> 4) ^ 
           (scene >> 5) ^ (scene >> 7)) & 1);
  set_scene();
}

// Move one screen to the left
void left_scene() {
  scene = (scene >> 1) | 
           ((((scene >> 4) ^ (scene >> 5) ^ (scene >> 6) ^ 
            (scene >> 1)) & 1) << 7);
  set_scene();
}

// Main entry point  
void main() {
  reg_uart_clkdiv = 139; // 115,200 baud

  // Initialise the LCD and clear whole screen to black
  lcd_init();
  lcd_clear_screen(0);

  uint8_t buttons = 0, old_buttons;

#ifdef copyright
  draw_copyright();
#endif 

  int min = 20;
  int sec = 0;
  int  score = 2000;

  int h = 0;
  bool flip = false, quick_flip = false, slow_flip = false;
  uint32_t frame_counter = 0;
  
  underground = false;

  scene = 0xc4;

  harry_x = 8;
  harry_dir = FORWARDS;

  // Main game loop
  while(true) {
    // Redraw top of screen  
    draw_top();
  
    // Draw leaves
    draw_leaves();

#ifdef scene_number
    // Draw the scene number
    show_score(100, 2, scene, colors[WHITE]);
#endif

    // draw the counting down time
    draw_time(20, 12, min, sec, colors[WHITE]);

    // Draw lives
    show_lives(10, 12, 2);

    // Show the score
    show_score(20, 2, score, colors[WHITE]);
  
    // Redraw the ground and underground and trees
    draw_lower_ground();
    draw_underground();
    draw_trees();
    
    // Set the default pool color and default to no pool
    pool_color = colors[BLACK];
    for(int i=0;i<7;i++) ground_pf[i] = 0;
 
    // Draw scene-specific items
    if (scene_type < 2) { // Hole scene 
      for(int i=1;i<6;i++) 
        ground_pf[i] = (scene_type == HOLE3_SCENE ? 0x000E1 : 0x00001);
      
      draw_ground();

      // Draw ladder
      draw_ladder(76, false);
      
      // Draw wall, left or right
      draw_wall((scene & 0x80) ? 136 : 17, false);
    
    } else {
      if (scene_type == 2 || scene_type == 4) {
        // Ensure pool of correct colour is drawn
        for(int i=0;i<6;i++) ground_pf[i] = pool[i];
        if (scene_type == 2) pool_color = colors[BLUE];
      }
      
      draw_vine();
      draw_ground();
    }

    uint8_t rolling = 148;
    bool jumping = false;
    bool got_treasure = false;

    // Move harry across the screen    
    while (harry_x >= HARRY_MIN && harry_x <= HARRY_MAX) {    

      // Draw harry
      draw_harry(harry_x, (jumping ? 88 : 96) + (underground ? 56 : 0), 
                 jumping ? harry[1] : harry[4-h], flip);
  
      // Draw treasure etc.
      if (scene_type == 5) {
        if (harry_x == 124 && !got_treasure) {
          show_score(20, 2, score, colors[DARK_GREEN]); //undraw 
          score += 4000;
          show_score(20, 2, score, colors[WHITE]);
          got_treasure = true;
        }

        // Draw the treasure
        if (!got_treasure) {
          if (object_type == 1) 
            draw_object(124, 112, colors[PURPLE], colors[YELLOW], money_bag, false);
          else if (object_type == 2) 
                 draw_object(124, 112, colors[PURPLE], colors[YELLOW], ring, false);
          else draw_object(124, 112, colors[GOLD], colors[YELLOW], 
                              (quick_flip ? bar0 : bar1), false);
        }
      } else {  
        switch(object_type) {
          case 4:
          case 3:
            if (rolling < 112)
              draw_object(rolling + 48, 112, colors[BROWN], colors[YELLOW],
                          quick_flip ? log2 : log1, false);
          case 2:
          case 1:
            // Draw rolling log
            if (rolling < 136)
               draw_object(rolling + 24, 112, colors[BROWN], colors[YELLOW],
                           quick_flip ? log2 : log1, false);
          case 0:
            draw_object(object_type == 0 ? 124: rolling, 112, 
                        colors[BROWN], colors[YELLOW], 
                        quick_flip && object_type != 0 ? log2 : log1, false);
            break;
          case 6:
            // Draw the fire
            draw_object_2col(124, 112, colors[RED], colors[BROWN], 
                         (quick_flip ? fire0 : fire1));
            break;
          case 7:
            // Draw the cobra
            draw_object(124, 112, colors[BLACK], colors[YELLOW],
                             (quick_flip ? cobra0 : cobra1), false );
            break;
        }
      }

      if (scene_type > 1) {
        // Draw the patrolling scorpion
        draw_object(slow_flip ? 103 - (harry_x & 0xf)  : 80 + (harry_x & 0xf), 
                  160, colors[WHITE], 0, (quick_flip ? scorpion0 : scorpion1), slow_flip);
      }

      // Draw the crocodiles
      if (scene_type == CROCO_SCENE) 
        for(int i=0;i<3;i++) 
          draw_object(60 + (i*20), 112, colors[DARK_GREEN], pool_color,
                           (slow_flip ? croc0 : croc1), false);

      //delay(50);

      if ((++frame_counter & 0xf) == 0xf) {
        // Update the time
        if (sec-- == 0) {
          min--;
          sec = 59;
        }
      }

      // Draw the countdown timer
      draw_block(20, 12, 100, 8, colors[DARK_GREEN]);
      draw_time(20, 12, min, sec, colors[WHITE]);
    
      // Undraw harry
      undraw_harry(harry_x, (jumping ? 88 : 96) + (underground ? 56: 0), 
                   jumping ? harry[1] : harry[4-h], flip);

      // If underground, redraw the ladder
      if (underground && scene_type < 2 &&
          (harry_x >= 67 && harry_x <= 83)) draw_ladder(76, false);
   
      // Redraw the ground to wipe out rolling logs etc.
      draw_ground();

      if (scene_type > 1) {  
        // Undraw the scorpion
        draw_block(slow_flip ? 103 - (harry_x & 0xf) : 80 + (harry_x & 0xf), 
                   160, 16, 16, 0);
      }

      quick_flip = !quick_flip;
      if ((harry_x & 0xf) == 0xf) slow_flip = !slow_flip;
      rolling = (rolling <= 8 ? 124 : rolling - 4);

      // Fall down hole
      if (scene_type == 1 && !jumping) {
        if ((harry_x > 50 && harry_x < 55) || (harry_x > 100 && harry_x < 105)) {
          underground = true;
        }
      }

      // Check input
      buttons = reg_buttons;
 
      // Climb down ladder
      if (scene_type < 2 && !jumping) {
        if (harry_x > 78 && harry_x < 82) {
          underground = true;
          if (!(buttons & BUTTON_DOWN)) {
            show_score(20, 2, score, colors[DARK_GREEN]); //undraw 
            if (score >= 100) score -= 100; 
            show_score(20, 2, score, colors[WHITE]); 
          }
        }
      }

      // Climb up the ladder
      if (underground && (harry_x > 76 && harry_x < 82)) {
        if (buttons & BUTTON_UP) underground = false;
      }

      // Change direction
      if (buttons & BUTTON_LEFT) {
        harry_dir = BACKWARDS;
      } else if (buttons & BUTTON_RIGHT) {
        harry_dir = FORWARDS;
      } 

      // Start jump
      if (!jumping && (buttons & BUTTON_UP)) {
        jumping = true;
        h = 0;
      }

      flip = (harry_dir == BACKWARDS);

      // Finish jump
      if (h++ == (jumping ? 8 : 4)) {
        h = 0;
        jumping = false;
      } 

      // Move harry
      if (harry_dir == FORWARDS) harry_x++;
      else if (harry_dir == BACKWARDS) harry_x--;
    
      // Check for hitting wall
      if (underground) {
        if (scene & 0x80) {
          if (harry_dir == FORWARDS && harry_x == 128) harry_x -= 8;
        } else {
          if (harry_dir == BACKWARDS && harry_x == 25) harry_x += 8;
        }
      }
    }

    if (scene_type < 2) { // Hole scene 
      // Undraw ladder
      draw_ladder(76, true);
      
      // Undraw wall, left or right
      draw_wall((scene & 0x80) ? 136 : 17, true);
    }

    // Move to next scene
    if (harry_dir == FORWARDS) {
      if (harry_x > HARRY_MAX) {
        right_scene();
        if (underground) {
          right_scene();
          right_scene();
        }
        harry_x = HARRY_MIN;
        h = 0;
      }
    } else if (harry_dir == BACKWARDS) {
      if (harry_x < HARRY_MIN) {
        left_scene;
        if (underground) {
          left_scene();
          left_scene();
        }
        harry_x = HARRY_MAX;
        h = 0;
      }
    }
  }
}

