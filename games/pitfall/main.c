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
};


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

uint32_t leaves[] = {
    0xffffe,
    0xffffc,
    0x3fcfc,
    0x07838
};

uint32_t pool[] = {
    0x0000F,
    0x0003F,
    0x000FF,
    0x003FF,
    0x000FF,
    0x0003F,
    0x0000F
};

uint8_t *numbers[] = {zero, one, two, three, four, five, six, seven, eight, nine};

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

void draw_top() {
  lcd_set_window(0, Y_OFFSET, WIDTH-1, HEIGHT-1);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;

  reg_pf = 0;
  reg_color = colors[0] << 16;
  reg_room = 15360;
}

void draw_leaves() {
  lcd_set_window(0, Y_OFFSET + 48, WIDTH-1, HEIGHT-1);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;

  reg_color = (colors[2] << 16) | colors[0];

  for(int i=0;i<4;i++) {
    reg_pf = leaves[i];
    reg_room = 1280;
  }
}

void draw_trees() {
  lcd_set_window(0, Y_OFFSET + 64, WIDTH-1, HEIGHT-1);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;

  reg_pf = 0x01010;
  reg_color = (colors[2] << 16) | colors[1];
  reg_room = 15360;
}

void draw_pool() {
  lcd_set_window(0, Y_OFFSET + 112, WIDTH-1, HEIGHT-1);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;

  reg_color = (colors[3] << 16) | colors[5];

  reg_pf = 0;
  reg_room = 320;
  for(int i=0;i<7;i++) {
    reg_pf = pool[i];
    reg_room = 640;
  }

  reg_pf = 0;
  reg_room = 320;

  reg_pf = 0;
  reg_color = (colors[4] << 16);
  reg_room = 5120;
}

void draw_underground() {
  lcd_set_window(0, Y_OFFSET + 176, WIDTH-1, HEIGHT-1);
  lcd_send_cmd(ILI9341_MEMORYWRITE);
  reg_dc = 1;

  reg_pf = 0;
  reg_color = (colors[4] << 16) | colors[1];
  reg_room = 2560;
}

// Draw object
void draw_object(uint8_t x, uint8_t y, uint16_t oc, const uint8_t *d, bool flip) {
  for(int n=0;n<16;n++) {
    uint8_t c = *(d + 15);
    for(int i=0;i<16;i++) {
      for(int j=0;(c & (flip ? 1 : 0x80)) && j<2;j++) 
        lcd_draw_pixel(((x + i) <<1) + j, Y_OFFSET + y, oc);
      if (flip) c >>= 1; else c <<= 1;
    }
    d--;
    y++;
  }
}

// Draw object with two colours
void draw_object2(uint8_t x, uint8_t y, uint16_t oc1, uint16_t oc2,
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
  for(int n=0;n<22;n++) {
    int col = (n < 2 ? 1 : (n < 6 ? 9 : (n < 12 ? 5 : 0)));
    uint8_t c = *(d + 21);
    for(int i=0;i<16;i++) {
      for(int j=0;(c & (flip ? 1 : 0x80)) && j<2;j++) 
        lcd_draw_pixel(((x + i) <<1) + j, Y_OFFSET + y, colors[y < 112 ? 2 : 3]);
      if (flip) c >>= 1; else c <<= 1;
    }
    d--;
    y++;
  }
}

// Draw number
void draw_number(uint8_t x, uint8_t y, uint16_t oc, const uint8_t *d, bool flip) {
  for(int n=0;n<8;n++) {
    uint8_t c = *(d + (flip ? 7 : 0));
    for(int i=0;i<8;i++) {
      for(int j=0;(c & 0x80) && j<2;j++) 
        lcd_draw_pixel(((x + i) <<1) + j, Y_OFFSET + y, oc);
      c <<= 1;
    }
    d += (flip ? -1 : 1);
    y++;
  }
}

void draw_copyright() {
  draw_number(40, 194, colors[WHITE], f, false);
  draw_number(50, 194, colors[WHITE], p, false);
  draw_number(60, 194, colors[WHITE], g, false);
  draw_number(70, 194, colors[WHITE], c, false);
}

const int divisor[] = {10000,1000,100,10};

// Display score, hi-score or another numnber
void show_score(int x, int y, int score) {
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
    uint8_t *data = blank && i != 4 ? space : numbers[d];
    draw_number(x + (i*10), y, colors[7], data, true); 
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
    uint8_t *data = numbers[d];
    draw_number(x + (i*10), y, col, data, true); 
  }
}

void draw_time(int x, int y, int min, int sec, uint16_t col) {
  show_two_digits(x, y, min, col);
  draw_number(x + 20, y, col, colon, true);
  show_two_digits(x + 30, y, sec, col);
}

void show_lives(int x, int y, int lives) {
  for(int i=0;i<lives;i++) {
    for(int j=0;j<8;j++) {
      for(int k =0; k<2; k++) {
        lcd_draw_pixel(((x + (i*4)) << 1) + k, Y_OFFSET + y + j, colors[7]);
      }
    }
  }
}


void draw_ladder(int x) {
  for(int i=0;i<16;i++) 
    for(int j=0;j<6;j++)
      lcd_draw_pixel((x << 1) + i, Y_OFFSET + 117 + j, 0);   
  

  for(int n=0;n<4;n++) {
    for(int i=0;i<16;i++) 
      for(int j=0;j<2;j++)
        lcd_draw_pixel((x << 1) + i, Y_OFFSET + 128 + 4*n + j, 0);   

    for(int i=0;i<4;i++) 
      for(int j=0;j<4;j++) {
        lcd_draw_pixel((x << 1) + i, Y_OFFSET + 128 + 4*n + j, 0);   
        lcd_draw_pixel((x << 1) + i + 12, Y_OFFSET + 128 + 4*n + j, 0);
      }
  }

  for(int n=0;n<8;n++) {
    for(int i=0;i<8;i++) 
      for(int j=0;j<2;j++)
        lcd_draw_pixel((x << 1) + 4 + i, Y_OFFSET + 144 + 4*n + j, colors[3]);   
  }
}

void draw_wall(int x) {
  draw_object(x, 144, colors[8], wall, false);
  draw_object(x, 160, colors[8], wall, false);

  for(int n=0;n<8;n++) 
    for(int i=0;i<15;i++) 
      lcd_draw_pixel((x << 1) + i, Y_OFFSET + 147 + 4*n, colors[10]);
}


// Main entry point  
void main() {
  reg_uart_clkdiv = 139; // 115,200 baud

  // Initialise the LCD and clear whole screen to black
  lcd_init();
  lcd_clear_screen(0);

  // Draw screen
  draw_top();
  draw_leaves();
  draw_trees();
  draw_pool();
  draw_underground();

  show_lives(10, 12, 2);
  show_score(20, 2, 2000);

  uint8_t buttons = 0, old_buttons;

  draw_wall(120);
  draw_ladder(30);

  draw_copyright();
 
  int min = 20;
  int sec = 0;

  int h = 0;
  bool flip = false, quick_flip = false;

  uint32_t counter = 0;

  // Main game loop
  while(true) {
    counter ++;

    // undraw the time
    draw_time(20, 12, min, sec, colors[DARK_GREEN]);

    // undraw harry
    undraw_harry(120, 96, harry[h], flip);
 
    // undraw the scorpion
    draw_object((flip ? 107 - h : 100 + h), 160, colors[BLACK], 
                (quick_flip ? scorpion0 : scorpion1), flip);

    // undraw the fire
    draw_object(10, 112, colors[YELLOW], (quick_flip ? fire0 : fire1), false);

    // undraw the rolling log
    draw_object(20, 112, colors[YELLOW], (quick_flip ? log1 : log2), false);

    // undraw the cobra
    draw_object(150, 112, colors[YELLOW], (quick_flip ? cobra0 : cobra1), false );

    // undraw the treasure
    if (h ==1) draw_object(140, 112, colors[YELLOW], money_bag, false);
    else if (h == 2) draw_object(140, 112, colors[YELLOW], ring , false);
    else draw_object(140, 112, colors[YELLOW], (quick_flip ? bar0 : bar1), false);

    // undraw the crocodiles
    for(int i=0;i<3;i++) 
      draw_object(60 + (i*20), 112, colors[BLUE], (quick_flip ? croc0 : croc1), false);

    if (sec-- == 0) {
      min--;
      sec = 59;
    }

    if (h++ == 8) {
      h = 0;
      flip = !flip;
    }

    quick_flip = !quick_flip;
     
    // draw the counting down time
    draw_time(20, 12, min, sec, colors[WHITE]);

    // draw harry doing irish dancing
    draw_harry(120, 96, harry[h], flip);

    // draw the patrolling scorpion
    draw_object((flip ? 107 - h : 100 + h), 160, colors[WHITE], 
                (quick_flip ? scorpion0 : scorpion1), flip);

    // draw the fire
    draw_object2(10, 112, colors[RED], colors[BROWN], (quick_flip ? fire0 : fire1));

    // draw the rolling log
    draw_object(20, 112, colors[YELLOW_GREEN], (quick_flip ? log1 : log2), false);

    // draw the cobra
    draw_object(150, 112, colors[BLACK], (quick_flip ? cobra0 : cobra1), false);

    // draw the treasure
    if (h == 1) draw_object(140, 112, colors[PURPLE], money_bag, false);
    else if (h == 2) draw_object(140, 112, colors[PURPLE], ring, false);
    else draw_object(140, 112, colors[PURPLE], (quick_flip ? bar0 : bar1), false);

    // draw the crocodiles
    for(int i=0;i<3;i++) 
      draw_object(60 + (i*20), 112, colors[BROWN], (quick_flip ? croc0 : croc1), false);
    
    delay(1000);
  }
}

