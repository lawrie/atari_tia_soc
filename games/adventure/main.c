#include <stdint.h>
#include <stdbool.h>
#include <uart/uart.h>
#include <ili9341/ili9341.h>
#include <delay/delay.h>
#include <button/button.h>

#define reg_uart_clkdiv (*(volatile uint32_t*)0x02000004)
#define reg_buttons (*(volatile uint32_t*)0x03000004)
#define reg_uart_data (*(volatile uint32_t*)0x02000008)

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 192

#define Y_OFFSET 24

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// Colours
const uint16_t colors[] = {
                     0xBDB6, // Grey
                     0xB216, // Purple
                     0xFFE0, // Yellow,
                     0xF100, // Red
                     0x07E0, // Green
                     0x001F, // Blue
                     0x0000, // Black
                     0xFFFF, // White
                     0x3F4C  // Light Green
};

// Room data
uint32_t number_room[] = {

       0xFFFFF,          //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t castle[] = {
       0xFFEA8,          //XXXXXXXXXXX X X X      R R R RRRRRRRRRRR
       0xC03F8,          //XX        XXXXXXX      RRRRRRR        RR
       0xC03FF,          //XX        XXXXXXXXXXRRRRRRRRRR        RR
       0xC00FF,          //XX          XXXXXXXXRRRRRRRR          RR
       0xC00FC,          //XX          XXXXXX    RRRRRR          RR
       0xC0000,          //XX                                    RR
       0xFFFC0           //XXXXXXXXXXXXXX            RRRRRRRRRRRRRR
};

uint32_t below_yellow[] = {
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t below_yellow_flip[] = {
       0xFFFF0,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0xFFFFF           //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
};

uint32_t top_entry[] = {
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xFFFFF           //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
};

uint32_t side_corridor[] = {
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t two_exit[] = {
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t left_of_name[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t white_castle_entry[] = {
       0xF3FCC,          //XXXX  XXXXXX  XX        RR  RRRRRR  RRRR
       0xF000C,          //XXXX          XX        RR          RRRR
       0xF0FFF,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00300,          //      XX                        RR
       0xF0300,          //XXXX  XX                        RR  RRRR
       0x00300,          //      XX                        RR
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t blue_maze_top[] = {
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00C30,          //        XX    XX        RR    RR
       0xF0C3C,          //XXXX    XX    XXXX    RRRR    RR    RRRR
       0xF0C00,          //XXXX    XX                    RR    RRRR
       0xFFFFC,          //XXXXXXXXXXXXXXXXXX    RRRRRRRRRRRRRRRRRR
       0x0300C,          //      XX        XX    RR        RR
       0xF33FC           //XXXX  XX  XXXXXXXX    RRRRRRRR  RR  RRRR
};

uint32_t blue_maze_1[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
       0x00000,          //
       0xFFCFF,          //XXXXXXXXXX  XXXXXXXXRRRRRRRR  RRRRRRRRRR
       0xF0003,          //XXXX              XXRR              RRRR
       0xF3FF3,          //XXXX  XXXXXXXXXX  XXRR  RRRRRRRRRR  RRRR
       0x03033,          //      XX      XX  XXRR  RR      RR
       0xFF333           //XXXXXXXX  XX  XX  XXRR  RR  RR  RRRRRRRR
};

uint32_t blue_maze_bottom[] = {
       0xFF330,          //XXXXXXXX  XX  XX        RR  RR  RRRRRRRR
       0x03030,          //      XX      XX        RR      RR
       0xF3FF0,          //XXXX  XXXXXXXXXX        RRRRRRRRRR  RRRR
       0xF0000,          //XXXX                                RRRR
       0xFF000,          //XXXXXXXX                        RRRRRRRR
       0x03000,          //      XX                        RR
       0xFFFFF           //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
};

uint32_t blue_maze_center[] = {
       0xF33FC,           //XXXX  XX  XXXXXXXX    RRRRRRRR  RR  RRRR
       0x0303C,           //      XX      XXXX    RRRR      RR
       0xFFF3C,           //XXXXXXXXXXXX  XXXX    RRRR  RRRRRRRRRRRR
       0x0033C,           //          XX  XXXX    RRRR  RR
       0XF333C,           //XXXX  XX  XX  XXXX    RRRR  RR  RR  RRRR
       0x03330,           //      XX  XX  XX        RR  RR  RR
       0xFF330            //XXXXXXXX  XX  XX        RR  RR  RRRRRRRR
};

uint32_t blue_maze_entry[] = {
       0xFF333,          //XXXXXXXX  XX  XX  XXRR  RR  RR  RRRRRRRR
       0x03330,          //      XX  XX  XX        RR  RR  RR
       0xF333F,          //XXXX  XX  XX  XXXXXXRRRRRR  RR  RR  RRRR
       0x03300,          //      XX  XX                RR  RR
       0xFF3FF,          //XXXXXXXX  XXXXXXXXXXRRRRRRRRRR  RRRRRRRR
       0x00000,          //
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

// Unused rooms in level 1
uint32_t black_maze_1[] = {
       0xFF0FF,          //XXXXXXXX    XXXXXXXXRRRRRRRR    RRRRRRRR
       0x00003,          //            XX            RR
       0xFFFFC,          //XXXXXXXXXXXXXX            RRRRRRRRRRRRRR
       0x00000,          //
       0xC3000,          //XX    XXXXXXXXXXXXXXRRRRRRRRRRRRRR    RR
       0x03000,          //      XX                        RR 
       0xFF0FF           //XXXXXXXX    XXXXXXXXRRRRRRRR    RRRRRRRR
};

uint32_t black_maze_2[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXXXMMMMMMMMMMMMMMMMMMMM
       0x00003,          //                  XX                  MM
       0xFFFF3,          //XXXXXXXXXXXXXXXX  XXMMMMMMMMMMMMMMMM  MM
       0x0003C,          //                  XX                  MM
       0xF0FFF,          //XXXX    XXXXXXXXXXXXMMMM    MMMMMMMMMMMM
       0x00F03,          //        XXXX      XX        MMMM      MM
       0xCCF33           //XX  XX  XXXX  XX  XXMM  MM  MMMM  MM  MM
};

uint32_t black_maze_3[] = {
       0xFF0FF,          //XXXXXXXX    XXXXXXXXRRRRRRRR    RRRRRRRR
       0xC0000,          //XX                  MM
       0xC3FFF,          //XX    XXXXXXXXXXXXXXMM    MMMMMMMMMMMMMM
       0x03000,          //      XX                  MM
       0xFF0FF,          //XXXXXXXX    XXXXXXXXMMMMMMMM    MMMMMMMM
       0xC00C0,          //XX          XX      MM          MM
       0xFF0FF           //XXXXXXXX    XXXXXXXXMMMMMMMM    MMMMMMMM
};

uint32_t black_maze_entry[] = {
       0xCCF33,          //XX  XX  XXXX  XX  XXMM  MM  MMMM  MM  MM
       0x00C03,          //        XX        XXRR        RR
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00000,          //
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00000,          //
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t maze_middle[] = {
       0xFFF33,          //XXXXXXXXXXXX  XX  XXRR  RR  RRRRRRRRRRRR
       0x00033,          //              XX  XXRR  RR
       0xF03F3,          //XXXX      XXXXXX  XXRR  RRRRRR      RRRR
       0x00300,          //          XX                RR
       0xFF33F,          //XXXXXXXX  XX  XXXXXXRRRRRR  RR  RRRRRRRR
       0x03330,          //      XX  XX  XX        RR  RR  RR 
       0xF3333           //XXXX  XX  XX  XX  XXRR  RR  RR  RR  RRRR
};

uint32_t maze_entry[] = {
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x0C000,          //      XX                        RR
       0xF30FF,          //XXXX  XX    XXXXXXXXRRRRRRRRR   RR  RRRR
       0x03003,          //      XX          XXRR          RR
       0xFF303,          //XXXXXXXX  XX      XXRR      RR  RRRRRRRR
       0x00303,          //          XX      XXRR      RR
       0xFFF33,          //XXXXXXXXXXXX  XX  XXRR  RR  RRRRRRRRRRRR
};

uint32_t maze_side[] = {
       0xF3333,          //XXXX  XX  XX  XX  XXRR  RR  RR  RR  RRRR
       0x03033,          //      XX      XX  XXRR  RR      RR
       0x03F33,          //      XXXXXX  XX  XXRR  RR  RRRRRR
       0x00003,          //                  XXRR
       0x03FC3,          //      XXXXXXXX    XXRR    RRRRRRRR
       0x03003,          //      XX          XXRR          RR
       0xFFFFF           //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR 
};

uint32_t red_maze_1[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
       0x00000,          //
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00003,          //                  XX        RR
       0xFFFF3,          //XXXXXXXXXXXX  XX        RR  RRRRRRRRRRRR
       0xF0333,          //XXXX      XX  XX  XXRR  RR  RR      RRRR
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t red_maze_top[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
       0x00003,          //                  XXRR
       0xFFFF3,          //XXXXXXXXXXXXXXXX  XXRR  RRRRRRRRRRRRRRRR
       0x00033,          //              XX  XXRR  RR
       0xF33FF,          //XXXX  XX  XXXXXXXXXXRRRRRRRRRR  RR  RRRR
       0xF3300,          //XXXX  XX  XX                RR  RR  RRRR
       0xF33F3           //XXXX  XX  XXXXXX  XXRR  RRRRRR  RR  RRRR
};

uint32_t red_maze_bottom[] = {
       0xF33F3,          //XXXX  XX  XXXXXX  XXRR  RRRRRR  RR  RRRR
       0xF3000,          //XXXX  XX                        RR  RRRR
       0xF33FF,          //XXXX  XX  XXXXXXXXXXRRRRRRRRRR  RR  RRRR
       0x03300,          //      XX  XX                RR  RR  RRRR
       0xFFF00,          //XXXXXXXXXXXX                RRRRRRRRRRRR
       0x00000,          //
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

// Room numbers
#define BLACK_CASTLE 0x10
#define YELLOW_CASTLE 0x11
#define YELLOW_CASTLE_ENTRY 0x12
#define GREEN_DRAGON_ROOM 0x1D
#define NAME_ROOM 0x1E

// Room table
typedef struct Rooms {
  uint32_t *data;
  uint8_t color;
  uint8_t pf_control;
  uint8_t above, left, down, right;
  bool port_open;
} Rooms;

Rooms rooms[] = {
  {number_room,        1, 0x00, 0x00, 0x00, 0x00, 0x00, false}, // 00 Number room
  {below_yellow_flip,  8, 0x01, 0x08, 0x02, 0x00, 0x02, false}, // 01 Under Blue Maze
  {below_yellow,       4, 0x00, 0x11, 0x01, 0x00, 0x03, false}, // 02 Under Yellow Castle
  {left_of_name,       2, 0x00, 0x00, 0x02, 0x1D, 0x1E, false}, // 03 Left of Name
  {blue_maze_top,      5, 0x00, 0x10, 0x05, 0x07, 0x05, false}, // 04 Top of Blue Maze
  {blue_maze_1,        5, 0x00, 0x1D, 0x04, 0x08, 0x06, false}, // 05 Blue Maze #1
  {blue_maze_bottom,   5, 0x00, 0x07, 0x05, 0x03, 0x05, false}, // 06 Bottom of Blue Maze
  {blue_maze_center,   5, 0x00, 0x04, 0x08, 0x06, 0x08, false}, // 07 Center of Blue Maze
  {blue_maze_entry,    5, 0x00, 0x05, 0x07, 0x01, 0x07, false}, // 08 Blue Maze Entry
  {maze_middle,        2, 0x00, 0x0A, 0x0A, 0x0B, 0x0A, false}, // 09 Maze Middle
  {maze_entry,         2, 0x00, 0x03, 0x09, 0x09, 0x09, false}, // 0A Maze Entry
  {maze_side,          2, 0x00, 0x09, 0x0C, 0x1C, 0x0D, false}, // 0B Maze Side
  {side_corridor,      2, 0x00, 0x1C, 0x0C, 0x1D, 0x0B, false}, // 0C
  {side_corridor,      2, 0x00, 0x0F, 0x0B, 0x0E, 0x0C, false}, // 0D
  {top_entry,          2, 0x00, 0x0D, 0x10, 0x0F, 0x10, false}, // 0E
  {castle,             7, 0x00, 0x0E, 0x0F, 0x0D, 0x0F, false}, // OF White Castle
  {castle,             6, 0x00, 0x1B, 0x1C, 0x04, 0x1C, false}, // 10 Black Castle
  {castle,             2, 0x00, 0x12, 0x00, 0x02, 0x00, false}, // 11 Yellow Castle
  {number_room,        2, 0x00, 0x12, 0x12, 0x11, 0x12, false}, // 12 Yellow Castle Entry
  {black_maze_1,       6, 0x00, 0x15, 0x14, 0x15, 0x16, false}, // 13 Black Maze #1
  {black_maze_2,       2, 0x00, 0x16, 0x15, 0x16, 0x13, false}, // 14 Black Maze #2
  {black_maze_3,       2, 0x00, 0x13, 0x16, 0x13, 0x14, false}, // 15 Black Maze #3
  {black_maze_entry,   2, 0x00, 0x1B, 0x13, 0x10, 0x15, false}, // 16 Black Maze Entry
  {red_maze_1,         2, 0x00, 0x19, 0x16, 0x19, 0x18, false}, // 17 Red Maze #1
  {red_maze_top,       2, 0x00, 0x1A, 0x17, 0x1A, 0x17, false}, // 18 Top of Red Maze
  {red_maze_bottom,    2, 0x00, 0x17, 0x1A, 0x17, 0x1A, false}, // 19 Bottom of Red Maze
  {white_castle_entry, 3, 0x00, 0x18, 0x19, 0x18, 0x19, false}, // 1A White Castle Entry
  {two_exit,           3, 0x00, 0x1C, 0x89, 0x10, 0x89, false}, // 1B Black Castle Entry
  {number_room,        1, 0x00, 0x1D, 0x07, 0x1B, 0x08, false}, // 1C Other Purple Room
  {top_entry,          3, 0x00, 0x03, 0x00, 0x00, 0x03, false}, // 1D Green dragon room
  {below_yellow,       1, 0x00, 0x06, 0x01, 0x06, 0x03, false}  // 1E Name Room
};

// Object data
uint8_t drag0[] = {
       0x06,                  //     XX
       0x0F,                  //    XXXX
       0xF3,                  //XXXX  XX
       0xFE,                  //XXXXXXX
       0x0E,                  //    XXX
       0x04,                  //     X
       0x04,                  //     X
       0x1E,                  //   XXXX
       0x3F,                  //  XXXXXX
       0x7F,                  // XXXXXXX
       0xE3,                  //XXX   XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC7,                  //XX   XXX
       0xFF,                  //XXXXXXXX
       0x3C,                  //  XXXX
       0x08,                  //    X
       0x8F,                  //X   XXXX
       0xE1,                  //XXX    X
       0x3F,                  //  XXXXXX
       0x00
};

uint8_t drag1[] = {
       0x80,                  //X
       0x40,                  // X
       0x26,                  //  X  XX
       0x1F,                  //   XXXXX
       0x0B,                  //    X XX
       0x0E,                  //    XXX
       0x1E,                  //   XXXX
       0x24,                  //  X  X
       0x44,                  // X   X
       0x8E,                  //X   XXX
       0x1E,                  //   XXXX
       0x3F,                  //  XXXXXX
       0x7F,                  // XXXXXXX
       0x7F,                  // XXXXXXX
       0x7F,                  // XXXXXXX
       0x7F,                  // XXXXXXX
       0x3E,                  //  XXXXX
       0x1C,                  //   XXX
       0x08,                  //    X
       0xF8,                  //XXXXX
       0x80,                  //X
       0xE0,                  //XXX
       0x00
};

uint8_t drag2[] = {
       0x0C,                  //    XX
       0x0C,                  //    XX
       0x0C,                  //    XX
       0x0E,                  //    XXX
       0x1B,                  //   XX X
       0x7F,                  // XXXXXXX
       0xCE,                  //XX  XXX
       0x80,                  //X
       0xFC,                  //XXXXXX
       0xFE,                  //XXXXXXX
       0xFE,                  //XXXXXXX
       0x7E,                  // XXXXXX
       0x78,                  // XXXX
       0x20,                  //  X
       0x6E,                  // XX XXX
       0x42,                  // X    X
       0x7E,                  // XXXXXX
       0x00
};

uint8_t bat1[] = {
       0x81,                  //X      X
       0x81,                  //X      X
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xFF,                  //XXXXXXX
       0x5A,                  // X XX X
       0x66,                  // XX  XX
       0x00
};

uint8_t bat2[] = {
       0x01,                  //       X
       0x80,                  //X
       0x01,                  //       X
       0x80,                  //X
       0x3C,                  //  XXXX
       0x5A,                  // X XX X
       0x66,                  // XX  XX
       0xC3,                  //X    XX
       0x81,                  //X      X
       0x81,                  //X      X
       0x81,                  //X      X 
       0x00
};

uint8_t key[] = {
       0x07,                  //     XXX
       0xFD,                  //XXXXXX X
       0xA7,                  //X X  XXX
       0x00
};                     

uint8_t chalice[] = {
       0x81,                  //X      X
       0x81,                  //X      X
       0xC3,                  //XX    XX
       0x7E,                  // XXXXXX
       0x7E,                  // XXXXXX
       0x3C,                  //  XXXX
       0x18,                  //   XX
       0x18,                  //   XX
       0x7E,                  // XXXXXX
       0x00
};

uint8_t portcullis[] = {
       0xFE,                  //XXXXXXX
       0xAA,                  //X X X X
       0x00
};

uint8_t sword[] = {
       0x20,                  //  X
       0x40,                  // X
       0xFF,                  //XXXXXXXX
       0x40,                  // X
       0x20,                  //  X
       0x00
};

uint8_t magnet[] = {
       0x3C,                  //  XXXX
       0x7E,                  // XXXXXX
       0xE7,                  //XXX  XXX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0x00  
};

uint8_t bridge[] = {
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0x42,                  // X    X
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0xC3,                  //XX    XX
       0x00
};

uint8_t dot[] = {
       0x80,                  //X
       0x00
};

uint8_t one[] = {
       0x04,                  // X
       0x0C,                  //XX
       0x04,                  // X
       0x04,                  // X
       0x04,                  // X
       0x04,                  // X
       0x0E,                  //XXX
       0x00
};

uint8_t two[] = {
       0x0E,                  // XXX
       0x11,                  //X   X
       0x01,                  //    X
       0x02,                  //   X
       0x04,                  //  X
       0x08,                  // X
       0x1F,                  //XXXXX
       0x00
};

uint8_t three[] = {
       0x0E,                  // XXX
       0x11,                  //X   X
       0x01,                  //    X
       0x06,                  //  XX
       0x01,                  //    X
       0x11,                  //X   X
       0x0E,                  // XXX
       0x00
};

const uint8_t easter_egg[] = {
       0xF0,                 //XXXX
       0x80,                 //X
       0x80,                 //X
       0x80,                 //X
       0xF4,                 //XXXX X
       0x04,                 //     X
       0x87,                 //X    XXX
       0xE5,                 //XXX  X X
       0x87,                 //X    XXX
       0x80,                 //X
       0x05,                 //     X X
       0xE5,                 //XXX  X X
       0xA7,                 //X X  XXX
       0xE1,                 //XXX    X
       0x87,                 //X    XXX
       0xE0,                 //XXX
       0x01,                 //       X
       0xE0,                 //XXX
       0xA0,                 //X X
       0xF0,                 //XXXX
       0x01,                 //       X
       0x40,                 // X
       0xE0,                 //XXX
       0x40,                 // X
       0x40,                 // X
       0x40,                 // X
       0x01,                 //       X
       0xE0,                 //XXX
       0xA0,                 //X X
       0xE0,                 //XXX
       0x80,                 //X
       0xE0,                 //XXX
       0x01,                 //       X
       0x20,                 //  X
       0x20,                 //  X
       0xE0,                 //XXX
       0xA0,                 //X X
       0xE0,                 //XXX
       0x01,                 //       X
       0x01,                 //       X
       0x01,                 //       X
       0x88,                 //   X   X
       0xA8,                 //X X X
       0xA8,                 //X X X
       0xA8,                 //X X X
       0xF8,                 //XXXXX
       0x01,                 //       X
       0xE0,                 //XXX 
       0xA0,                 //X X 
       0xF0,                 //XXXX
       0x01,                 //       X
       0x80,                 //X
       0xE0,                 //XXX
       0x8F,                 //X   XXXX
       0x89,                 //X   X  X
       0x0F,                 //    XXXX
       0x8A,                 //X   X X
       0xE9,                 //XXX X  X
       0x80,                 //X
       0x8E,                 //X   XXX
       0x0A,                 //    X X
       0xEE,                 //XXX XXX
       0xA0,                 //X X
       0xE8,                 //XXX X
       0x88,                 //X   X
       0xEE,                 //XXX XXX
       0x0A,                 //    X X
       0x8E,                 //X   XXX
       0xE0,                 //XXX
       0xA4,                 //X X  X
       0xA4,                 //X X  X
       0x04,                 //     X
       0x80,                 //X
       0x08,                 //    X
       0x0E,                 //    XXX
       0x0A,                 //    X X
       0x0A,                 //    X X
       0x80,                 //X
       0x0E,                 //    XXX
       0x0A,                 //    X X
       0x0E,                 //    XXX
       0x08,                 //    X
       0x0E,                 //    XXX
       0x80,                 //X
       0x04,                 //     X
       0x0E,                 //    XXX
       0x04,                 //     X
       0x04,                 //     X
       0x04,                 //     X
       0x80,                 //X
       0x04,                 //     X
       0x0E,                 //    XXX
       0x04,                 //     X
       0x04,                 //     X
       0x04,                 //     X
       0x00
};

// Object states
#define IN_ROOM 0
#define CARRIED 1
#define DEAD 2

#define NUM_OBJECTS 11

// Object indices
#define BLACK_DOT 0
#define RED_DRAGON 1
#define YELLOW_DRAGON 2
#define GREEN_DRAGON 3
#define MAGNET 4
#define SWORD 5
#define CHALICE 6
#define BRIDGE 7
#define YELLOW_KEY 8
#define WHITE_KEY 9
#define BLACK_KEY 10

// Object table
typedef struct ObjectLocations {
  uint8_t *data;
  uint8_t room;
  uint8_t x;
  uint8_t y;
  uint8_t w;
  uint8_t h;
  uint8_t state;
  uint8_t color;
} ObjectLocations;

ObjectLocations locations[] = {
  {dot,     0x15, 0x51, 0x12, 1,  1,  0, 6},   // Black dot
  {drag0,   0x09, 0x50, 0x20, 8,  16, 0, 3},   // Red dragon
  {drag0,   0x01, 0x50, 0x20, 8,  16, 0, 2},   // Yellow dragon
  {drag0,   0x1D, 0x50, 0x50, 8,  16, 0, 4},   // Green dragon
  {magnet,  0x1B, 0x80, 0x20, 8,  4,  0, 6},   // Magnet
  {sword,   0x12, 0x20, 0x60, 8,  4,  0, 2},   // Sword
  {chalice, 0x1C, 0x30, 0x20, 8,  4,  0, 2},   // Chalice
  {bridge,  0x04, 0x29, 0x67, 16, 16, 0, 1},   // Bridge
  {key,     0x11, 0x20, 0x50, 8,  4,  0, 2},   // Yellow key
  {key,     0x0E, 0x20, 0x40, 8,  4,  0, 7},   // White key
  {key,     0x1D, 0x20, 0x20, 8,  4,  0, 6}    // Black key
};

// Working data 
uint8_t ball_x, ball_y;
bool started = false;
uint8_t current_room = 0;
uint8_t old_ball_x, old_ball_y;
int8_t carried = -1;
int8_t carried_x, carried_y;
uint16_t room_color, back_color;

// Get 20-bit wall data for given row
uint32_t get_wall(uint8_t y) {
  uint32_t *data = rooms[current_room].data;

  if (y < 16) return data[0];
  else return data[((y-16) >> 5) + 1];
}
 
// Get pixel corresponding to room co-ordinates 
bool get_pixel(uint8_t r, uint8_t x, uint8_t y) {
  uint32_t row = get_wall(y);
  uint32_t mask = 1 << ((x >=80 ? x-80 : 79-x) >> 2);
  return (row & mask);
}

// Draw room
void draw_room(int r) {
  Rooms room = rooms[r];

  for(int y=0; y<7; y++) {
    uint32_t bit = 0x80000;
    uint32_t pf = room.data[y];
    for(int x=0; x<20; x++) {
      for(int i=0; i<((y == 0 || y == 6) ? 16 : 32);i++) {
        uint16_t c = colors[room.color];
        int yy =  Y_OFFSET + (y == 0 ? i : 16 + i + ((y-1) * 32));
        uint16_t cc = pf & bit ? c : back_color;
        for(int j=0; j<8; j++) {
          int xx = (x*8) + j;
          lcd_draw_pixel(xx, yy , cc);
          lcd_draw_pixel(319 - xx, yy, cc);
        }
      }
      bit >>= 1;
    }
  }
}

// Draw player (ball)
void draw_ball(uint8_t x, uint8_t y, uint16_t c) {
  for(int i=0; i<8; i++) 
    for(int j=0;j<8 && y + j < 192;j++) 
      lcd_draw_pixel((x << 1) + i , Y_OFFSET + y + j, c);
}

// Draw object
void draw_object(uint8_t x, uint8_t y, uint16_t oc, const uint8_t *d, bool big) {
  uint8_t shift = (big ? 3 : 1);
  while(*d) {
    uint8_t c = *d;
    for(int i=0;i<8;i++) {
      for(int j=0;(c & 0x80) && j<(big ? 8 : 2);j++) 
        for(int k=0; k<(big ? 2 : 1) && y + k  < 192;k++) 
          lcd_draw_pixel((x<<1) + (i<<shift) + j, Y_OFFSET + y + k, oc);
      c <<= 1;
    }
    d++;
    y += (big ? 2 : 1);;
  }
}

// Draw object
void undraw_object(uint8_t x, uint8_t y, const uint8_t *d, bool big) {
  uint8_t shift = (big ? 3 : 1);
  uint16_t room_color = colors[rooms[current_room].color];

  while(*d) {
    uint8_t c = *d;
    for(int i=0;i<8;i++) {
      for(int j=0;(c & 0x80) && j<(big ? 8 : 2);j++) 
        for(int k=0; k<(big ? 2 : 1) && y + k  < 192;k++) {
          uint16_t xx = (x<<1) + (i<<shift) + j;
          uint8_t yy = y + k;
          bool pix = get_pixel(current_room, xx >> 1, yy);
          uint16_t color =  (pix ? room_color : back_color); 

          lcd_draw_pixel(xx, Y_OFFSET + yy, color);
        }
      c <<= 1;
    }
    d++;
    y += (big ? 2 : 1);;
  }
}

// Draw all objects in current room
void draw_objects() {
  for(int i=0; i<NUM_OBJECTS;i++) {
    ObjectLocations loc = locations[i];

    if (current_room == loc.room && loc.state != CARRIED)
      draw_object(loc.x, loc.y, colors[loc.color],
                  loc.data, i == BRIDGE);
  }
}

// Draw rung of portcullis
void draw_portcullis(int n) {
  for(int i=0;i<n;i++)
    draw_object(76, Y_OFFSET + 88 + (i<<1), 0, portcullis, false); 
}

// Animate opening of portcullis
void open_portcullis() {
  for(int i=15; i>=0; i--) {
    draw_object(76, Y_OFFSET + 88 + (i<<1), back_color, portcullis, false); 
    delay(50);
  }
}

// Check for collisions with walls 
bool check_collision(uint8_t direction) {
  uint32_t wall, mask;
  Rooms room = rooms[current_room];

  switch (direction) {
    case UP:
    case DOWN:
      wall = get_wall(ball_y + (direction == UP ? -1 : 8));
    
      if (ball_x < (SCREEN_WIDTH / 2) ) {
        for(int i=0;i<4; i++) {
          mask = 1 << (((SCREEN_WIDTH / 2) - 1 - i - ball_x) >> 2);
          if (wall & mask) return true;
        } 
      } else {
        for(int i=0;i<4; i++) {
          mask = 1 <<  ((ball_x + i - (SCREEN_WIDTH / 2)) >> 2);
          if (wall & mask) return true;
        }
      } 
      break;
    case LEFT:
      // Check for thin wall
      if ((room.pf_control & 1) && ball_x == 16) return true;
    
    case RIGHT:
      // Check for thin wall
      if (direction == RIGHT && 
          ((room.pf_control & 2) && ball_x == 138)) return true;
        
      if (ball_x < (SCREEN_WIDTH / 2)) {
        mask = 1 << (((direction == LEFT ? 80 : 75) - ball_x) >> 2);
      } else {
        mask = 1 << ((ball_x - (direction == LEFT ? 81 : 76)) >> 2);
      }

      for(int i=0;i<8; i++) {
        wall = get_wall(ball_y + i);
        if (wall & mask) return true;
      } 
      break;
  }
  return false; 
}

// Draw thin walls
void draw_thin_walls(bool left) {
  for(int i=0;i<192;i++)
    lcd_draw_pixel(left ? 31 : 286, Y_OFFSET + i, 0);
}

// Check if area touched
bool touched(uint8_t x0, uint8_t y0, uint8_t x, uint8_t y, int w, int h) {
  return (x0 >= x && x0 < x + w && y0 >= y && y0 < y + h);
}

// Check if ball touches an object rectangle
bool touched_object(uint8_t x, uint8_t y, int w, int h) {
  return touched(ball_x, ball_y, x, y , w, h) ||
         touched(ball_x + 4, ball_y, x, y, w, h) ||
         touched(ball_x, ball_y + 8, x, y, w, h) ||
         touched(ball_x + 4, ball_y + 8, x, y, w, h);
}

// Check if any object in current room is touched and returns its index
int8_t check_objects() {
  for(int i=0;i<NUM_OBJECTS;i++) {
    ObjectLocations loc = locations[i];

    if (loc.room == current_room && loc.state != CARRIED) 
      if (touched_object(loc.x, loc.y, loc.w, loc.h)) return i;
  }
  return -1;
}

// Move a specific dragon
void move_dragon(uint8_t obj) {
  ObjectLocations loc = locations[obj];

  if (loc.state == DEAD) return;

  draw_object(loc.x, loc.y, back_color, drag0, false);

  if (ball_x > loc.x) loc.x++;
  else if (ball_x < loc.x) loc.x--;

  if (ball_y > loc.y) loc.y++;
  else if (ball_y < loc.y) loc.y--;

  draw_object(loc.x, loc.y, colors[loc.color], drag0, false);
}

// Main entry point  
void main() {
  reg_uart_clkdiv = 139; // 115,200 baud

  // Initialise the LCD and clear whole screen to black
  lcd_init();
  lcd_clear_screen(0);

  // Grey background
  back_color = colors[0];

  // Start the ball under the portcullis
  ball_x = 78;
  ball_y = 148;

  uint8_t buttons = 0, old_buttons;
  uint8_t old_room;
  uint8_t level = 1, old_level;

  // Start in number room. Only level 1 currently available.
  draw_room(0);
  draw_object(76, 96, colors[4], one, false);

  // Main game loop
  while(true) {
    // Make copy of current room structure
    Rooms room = rooms[current_room];

    // See if we have won
    if (current_room == YELLOW_CASTLE_ENTRY && carried == CHALICE) {
      // Show room in multiple colours forever
      while(true) {
        for(int i=1;i<=8;i++) {
          rooms[current_room].color = i;
          draw_room(current_room);
        }
      }
    }

    // Save old values of data
    old_buttons = buttons;
    old_room = current_room;
    old_ball_x = ball_x;
    old_ball_y = ball_y;
    old_level = level;

    // Get button input
    buttons = reg_buttons;

    // Check for game started
    if ((buttons & BUTTON_X) && !(old_buttons & BUTTON_X)) {
      if (!started) {
        started = true;
        current_room = YELLOW_CASTLE;
      }
    }

    if (!started) {
      if ((buttons & BUTTON_Y) && !(old_buttons & BUTTON_Y)) {
        if (++level >  3) level = 1;

        undraw_object(76, 96, 
                      (old_level == 1 ? one : (old_level == 2 ? two: three)), false);

        draw_object(76, 96, colors[4], 
                    (level == 1 ? one : (level == 2 ? two : three)), false);
      }
     
      continue;
    }

    // Move dragons
    if (current_room == GREEN_DRAGON_ROOM) {
      //move_dragon(GREEN_DRAGON);
    }

    // Movement and collisions
    if ((buttons & BUTTON_UP) && !check_collision(UP)) {
      if (ball_y >= 2) ball_y--;
      else {
        uint8_t up = room.above;
        if (up > 0 && up <= 30) { 
           current_room = up;
           ball_y = 190;
        }
      }
    }

    if ((buttons & BUTTON_DOWN) && !check_collision(DOWN)) {
      if (ball_y < 190) ball_y++;
      else {
        uint8_t down = room.down;
        if (down > 0 && down <= 30) { 
           current_room = down;
           if (current_room == BLACK_CASTLE || current_room == YELLOW_CASTLE) ball_y = 160;
           else ball_y = 0; 
        }
      }
    }

    if ((buttons & BUTTON_LEFT) && !check_collision(LEFT)) {
      if (ball_x >= 2) ball_x--;
      else {
        uint8_t left = room.left;
        if (left > 0 && left <= 30) { 
           current_room = left;
           ball_x = 158; 
        }
      }
    } 

    if ((buttons & BUTTON_RIGHT) && !check_collision(RIGHT)) {
      if (ball_x <= 158) ball_x++;
      else {
        uint8_t right = room.right;
        if (right > 0 && right <= 30) { 
           current_room = right;
           ball_x = 0; 
        }
      }
    }
    
    // Drop object if button A pressed
    if ((buttons & BUTTON_A) && carried >= 0) {
      ObjectLocations *locp = &locations[carried];

      locp->state = IN_ROOM;
      locp->room = current_room;
      locp->x = ball_x + carried_x;
      locp->y = ball_y + carried_y;
      carried = -1;
    }

    // Check for pick-up of object
    int8_t obj = check_objects();

    if (obj >= 0) {
      ObjectLocations *locp = &locations[obj];
      // Check for killing dragon
      if (obj == GREEN_DRAGON || obj == YELLOW_DRAGON) {
        if (carried == SWORD) {
          locp->data = drag2;
          locp->state = DEAD;
          undraw_object(locp->x, locp->y,
                      drag0, false);
          draw_object(locp->x, locp->y, colors[locp->color],
                      drag2, false);
        } else {
          ball_x = old_ball_x;
          ball_y = old_ball_y;
        }
      } else if (locp->state == IN_ROOM) { 
        // Drop any existing object
        if (carried >= 0) {
          ObjectLocations *carlocp = &locations[carried];

          carlocp->state = IN_ROOM;
          carlocp->room = current_room;
          carlocp->x = ball_x + carried_x;
          carlocp->y = ball_y + carried_y;
        }

        // Pick up new object 
        carried = obj;
        undraw_object(locp->x, locp->y, 
                    locp->data, false);
        carried_x = locp->x - ball_x;
        carried_x += (carried_x > 0 ? 1 : -1);
        carried_y = locp->y - ball_y;
        carried_y += (carried_y > 0 ? 1 : -1);
        locp->state = CARRIED;
      }
    }

    // Open portcullis if carrying the key
    if (current_room == BLACK_CASTLE || current_room == YELLOW_CASTLE) {
      if (touched_object(76, 112, 8, 32)) {
        if (room.port_open) {
          current_room = room.above;
          ball_y = 190;
        } else if ((current_room == 0x10 && carried == BLACK_KEY) ||
            (current_room == YELLOW_CASTLE && carried == YELLOW_KEY)) {
          if (!room.port_open) {
            open_portcullis();
            rooms[current_room].port_open = true;
          }
          current_room = room.above;
          ball_y = 190;
        } else {
          // No entry
          ball_x = old_ball_x;
          ball_y = old_ball_y;
        }
      }
    }
 
    // Check for new room 
    if (current_room != old_room) {
      draw_room(current_room);
      room_color = colors[rooms[current_room].color];
      draw_ball(ball_x, ball_y, room_color);
      uint8_t thin_walls = rooms[current_room].pf_control;

      if (thin_walls & 3) draw_thin_walls(thin_walls & 1);

      if (current_room == NAME_ROOM) 
        draw_object(78, 0, colors[3], easter_egg, false);

      draw_objects();

      if (current_room >= BLACK_CASTLE && current_room <= YELLOW_CASTLE) { // Is it a castle
        if (!rooms[current_room].port_open) draw_portcullis(16);
      }
    }

    // Redraw stuff when ball moves
    if (ball_x != old_ball_x || ball_y != old_ball_y) {
      ObjectLocations carloc = locations[carried];

      if (current_room == old_room) {
        if (carried >= 0)
          undraw_object(old_ball_x + carried_x, old_ball_y + carried_y, 
                      carloc.data, false);
        draw_ball(old_ball_x, old_ball_y, back_color);
      }

      draw_ball(ball_x, ball_y, room_color);

      if (carried >= 0) 
        draw_object(ball_x + carried_x, ball_y + carried_y, 
                    colors[carloc.color], carloc.data, false);
    }
  }
}

