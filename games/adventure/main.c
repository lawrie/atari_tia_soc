#include <stdint.h>
#include <stdbool.h>
#include <uart/uart.h>
#include <ili9341/ili9341.h>
#include <delay/delay.h>
#include <button/button.h>

#define reg_uart_clkdiv (*(volatile uint32_t*)0x02000004)
#define reg_buttons (*(volatile uint32_t*)0x03000004)

#define reg_uart_data (*(volatile uint32_t*)0x02000008)

#define Y_OFFSET 24

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define NULL (void *)0

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

uint16_t room_color, back_color;

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
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0x00000,
       0xFFFF0           //XXXXXXXXXXXXXXXXX       RRRRRRRRRRRRRRRR
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
       0xFFFF0,          //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
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
       0xF033F,          //XXXX  XX  XX  XXXXXXRRRRRR  RR  RR  RRRR
       0x03300,          //      XX  XX                RR  RR
       0xFF3FF,          //XXXXXXXX  XXXXXXXXXXRRRRRRRRRR  RRRRRRRR
       0x00000,          //
       0xFFFF0           //XXXXXXXXXXXXXXXX        RRRRRRRRRRRRRRRR
};

uint32_t black_maze_1[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t black_maze_2[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t black_maze_3[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t black_maze_entry[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t maze_middle[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t maze_entry[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t maze_side[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t red_maze_1[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t red_maze_top[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

uint32_t red_maze_bottom[] = {
       0xFFFFF,          //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0x00000, 
       0xFFFFF           //XXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRRRR
};

typedef struct Rooms {
  uint32_t *data;
  uint8_t color;
  uint8_t pf_control;
  uint8_t above, left, down, right;
} Rooms;

Rooms rooms[] = {
  {number_room,        1, 0x00, 0x00, 0x00, 0x00, 0x00}, // 00 Number room
  {below_yellow_flip,  2, 0x02, 0x08, 0x02, 0x1D, 0x1E}, // 01 
  {below_yellow,       8, 0x00, 0x11, 0x03, 0x00, 0x01}, // 02
  {left_of_name,       4, 0x01, 0x08, 0x00, 0x00, 0x02}, // 03 
  {blue_maze_top,      5, 0x00, 0x10, 0x05, 0x07, 0x06}, // 04 Top of Blue Maze
  {blue_maze_1,        5, 0x00, 0x1D, 0x06, 0x08, 0x04}, // 05 Blue Maze #1
  {blue_maze_bottom,   5, 0x00, 0x07, 0x04, 0x03, 0x05}, // 06 Bottom of Blue Maze
  {blue_maze_center,   5, 0x00, 0x04, 0x08, 0x06, 0x08}, // 07 Center of Blue Maze
  {blue_maze_entry,    5, 0x00, 0x05, 0x07, 0x01, 0x07}, // 08 Blue Maze Entry
  {maze_middle,        2, 0x00, 0x0A, 0x0A, 0x0B, 0x0A}, // 09 Maze Middle
  {maze_entry,         2, 0x00, 0x03, 0x09, 0x09, 0x09}, // 0A Maze Entry
  {maze_side,          2, 0x00, 0x09, 0x0C, 0x1C, 0x0D}, // 0B Maze Side
  {side_corridor,      2, 0x00, 0x1C, 0x0C, 0x1D, 0x0B}, // 0C
  {side_corridor,      2, 0x00, 0x0F, 0x0B, 0x0E, 0x0C}, // 0D
  {top_entry,          2, 0x00, 0x0D, 0x10, 0x0F, 0x10}, // 0E
  {castle,             7, 0x00, 0x0E, 0x0F, 0x0D, 0x0F}, // OF White Castle
  {castle,             6, 0x00, 0x01, 0x1C, 0x04, 0x1C}, // 10 Black Castle
  {castle,             2, 0x00, 0x06, 0x03, 0x02, 0x01}, // 11 Yellow Castle
  {number_room,        2, 0x00, 0x12, 0x12, 0x12, 0x12}, // 12 Yellow Castle Entry
  {black_maze_1,       6, 0x00, 0x15, 0x14, 0x15, 0x16}, // 13 Black Maze #1
  {black_maze_2,       2, 0x00, 0x16, 0x15, 0x16, 0x13}, // 14 Black Maze #2
  {black_maze_3,       2, 0x00, 0x13, 0x16, 0x13, 0x14}, // 15 Black Maze #3
  {black_maze_entry,   2, 0x00, 0x14, 0x13, 0x1B, 0x15}, // 16 Black Maze Entry
  {red_maze_1,         2, 0x00, 0x19, 0x16, 0x19, 0x18}, // 17 Red Maze #1
  {red_maze_top,       2, 0x00, 0x1A, 0x17, 0x1A, 0x17}, // 18 Top of Red Maze
  {red_maze_bottom,    2, 0x00, 0x17, 0x1A, 0x17, 0x1A}, // 19 Bottom of Red Maze
  {white_castle_entry, 2, 0x00, 0x18, 0x19, 0x18, 0x19}, // 1A White Castle Entry
  {two_exit,           2, 0x00, 0x89, 0x89, 0x89, 0x89}, // 1B Black Castle Entry
  {number_room,        0, 0x00, 0x1D, 0x07, 0x8c, 0x08}, // 1C Other Purple Room
  {top_entry,          3, 0x00, 0x01, 0x01, 0x10, 0x03}, // 1D 
  {below_yellow,       2, 0x00, 0x06, 0x01, 0x06, 0x03}  // 1E Name Room
};

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

typedef struct ObjectLocations {
  uint8_t *data;
  uint8_t room;
  uint8_t x;
  uint8_t y;
  uint8_t color;
} ObjectLocations;

#define NUM_OBJECTS 11

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

ObjectLocations locations[] = {
  {dot,     0x15, 0x51, 0x12, 6}, // Black dot
  {drag0,   0x0E, 0x50, 0x20, 3}, // Red dragon
  {drag0,   0x09, 0x50, 0x20, 2}, // Yellow dragon
  {drag0,   0x1D, 0x50, 0x50, 4}, // Green dragon
  {magnet,  0x1B, 0x80, 0x20, 6}, // Magnet
  {sword,   0x12, 0x20, 0x20, 2}, // Sword
  {chalice, 0x1C, 0x30, 0x20, 2}, // Chalice
  {bridge,  0x04, 0x29, 0x37, 1}, // Bridge
  {key,     0x11, 0x20, 0x50, 2}, // Yellow key
  {key,     0x0E, 0x20, 0x40, 7}, // White key
  {key,     0x1D, 0x20, 0x20, 6}  // Black key
};
 
uint8_t ball_x, ball_y;
bool started = false;
uint8_t current_room = 0;
uint8_t old_ball_x, old_ball_y;
uint8_t key_x, key_y;

void draw_room(int r) {
  for(int y=0; y<7; y++) {
    uint32_t bit = 0x80000;
    uint32_t pf = rooms[r].data[y];
    for(int x=0; x<20; x++) {
      for(int i = 0; i< ((y == 0 || y == 6) ? 16 : 32);i++) {
        uint16_t c = colors[rooms[r].color];
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

void draw_ball(uint8_t x, uint8_t y, uint16_t c) {
  for(int i=0; i<8; i++) 
    for(int j=0;j<8 && y + j < 192;j++) 
      lcd_draw_pixel((x << 1) + i , Y_OFFSET + y + j, c);
}

void draw_object(uint8_t x, uint8_t y, uint16_t oc, uint16_t bc, const uint8_t *d) {
  while(*d) {
    uint8_t c = *d;
    for(int i=0;i<8;i++) {
      lcd_draw_pixel(x + i<<1, Y_OFFSET + y, c & 0x80 ? oc : bc);
      lcd_draw_pixel(x + 1 + i<<1, Y_OFFSET + y, c & 0x80 ? oc : bc);
      lcd_draw_pixel(x + i<<1, Y_OFFSET + y + 1, c & 0x80 ? oc : bc);
      lcd_draw_pixel(x + 1 + i<<1, Y_OFFSET + y + 1, c & 0x80 ? oc : bc);
      c <<= 1;
    }
    d++;
    y+= 2;
  }
}

void draw_objects() {
  for(int i=0; i<NUM_OBJECTS;i++) 
    if (current_room == locations[i].room && locations[i].data != NULL)
      draw_object(locations[i].x, locations[i].y, colors[locations[i].color],
                  back_color, locations[i].data);
}

void draw_portcullis(int n) {
  for(int i=0;i<n;i++)
    draw_object(76, Y_OFFSET + 88 + (i<<1), 0, back_color, portcullis); 
}

uint32_t get_wall(uint8_t y) {
  uint32_t *data = rooms[current_room].data;

  if (y < 16) return data[0];
  else return data[((y-16) >> 5) + 1];
}
  
bool check_collision(uint8_t direction) {
  uint32_t wall, wall1;

  switch (direction) {
    case UP:
      wall = get_wall(ball_y - 1);
      if (ball_x < 80) {
        uint32_t mask = 1 << ((79 - ball_x) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          mask >>= 1;
        } 
      } else {
        uint32_t mask = 1 <<  ((ball_x - 78) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          mask >>= 1;
        }
      } 
      break;
    case DOWN:
      wall = get_wall(ball_y + 8);
      if (ball_x < 80) {
        uint32_t mask = 1 << ((79 - ball_x) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          mask >>= 1;
        } 
      } else {
        uint32_t mask = 1 <<  ((ball_x - 78) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          mask >>= 1;
        }
      } 
      break;
    case LEFT:
      if ((rooms[current_room].pf_control & 1) && ball_x == 16) return true;
      wall = get_wall(ball_y);
      wall1 = get_wall(ball_y + 1);
      if (ball_x < 80) {
        uint32_t mask = 1 << ((80 - ball_x) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          wall = wall1;
        } 
      } else {
        uint32_t mask = 1 <<  ((ball_x - 81) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          wall = wall1;
        }
      } 
      break;
    case RIGHT:
      if ((rooms[current_room].pf_control & 2) && ball_x == 138) return true;
      wall = get_wall(ball_y);
      wall1 = get_wall(ball_y + 1);
      if (ball_x < 80) {
        uint32_t mask = 1 << ((75 - ball_x) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          wall = wall1;
        } 
      } else {
        uint32_t mask = 1 <<  ((ball_x - 76) >> 2);
        for(int i=0;i<2; i++) {
          if (wall & mask) return true;
          wall = wall1;
        }
      } 
      break;
  }
  return false; 
}

void draw_thin_walls(bool left) {
  for(int i=0;i<192;i++)
    lcd_draw_pixel(left ? 31 : 286, Y_OFFSET + i, 0);
}

bool touched(uint8_t x0, uint8_t y0, uint8_t x, uint8_t y, int w, int h) {
  return (x0 >= x && x0 <= x + w && y0 >= y && y0 <= y + h);
}

bool touched_object(uint8_t x, uint8_t y, int w, int h) {
  return touched(ball_x, ball_y, x, y , w, h) ||
         touched(ball_x + 4, ball_y, x, y, w, h) ||
         touched(ball_x, ball_y + 8, x, y, w, h) ||
         touched(ball_x + 4, ball_y + 8, x, y, w, h);
}
  
void main() {
  reg_uart_clkdiv = 139;

  lcd_init();

  lcd_clear_screen(0);

  back_color = colors[0];

  ball_x = 80;
  ball_y = 160;

  key_x = 0x20;
  key_y = 0x50;

  uint8_t buttons = 0, old_buttons;
  uint8_t old_room;

  draw_room(0);
  draw_object(76, 96, colors[4], back_color, one);

  while(true) {
    old_buttons = buttons;
    buttons = reg_buttons;

    old_room = current_room;

    old_ball_x = ball_x;
    old_ball_y = ball_y;

    if ((buttons & BUTTON_X) && !(old_buttons & BUTTON_X)) {
      if (!started) {
        started = true;
        current_room = 0x11;
      }
    }

    if (!started) continue;

    if ((buttons & BUTTON_UP) && !check_collision(UP)) {
      if (ball_y >= 2) ball_y--;
      else {
        uint8_t up = rooms[current_room].above;
        if (up > 0 && up <= 30) { 
           current_room = up;
           ball_y = 190; 
        }
      }
    }

    if ((buttons & BUTTON_DOWN) && !check_collision(DOWN)) {
      if (ball_y < 190) ball_y++;
      else {
        uint8_t down = rooms[current_room].down;
        if (down > 0 && down <= 30) { 
           current_room = down;
           ball_y = 0; 
        }
      }
    } 

    if ((buttons & BUTTON_LEFT) && !check_collision(LEFT)) {
      if (ball_x >= 2) ball_x--;
      else {
        uint8_t left = rooms[current_room].left;
        if (left > 0 && left <= 30) { 
           current_room = left;
           ball_x = 158; 
        }
      }
    } 

    if ((buttons & BUTTON_RIGHT) && !check_collision(RIGHT)) {
      if (ball_x <= 158) ball_x++;
      else {
        uint8_t right = rooms[current_room].right;
        if (right > 0 && right <= 30) { 
           current_room = right;
           ball_x = 0; 
        }
      }
    }

    if (current_room == 0x11) {
      if (touched_object(key_x, key_y, 16, 8)) {
        draw_object(key_x, key_y, colors[3], back_color, key);
      }
    }
  
    if (current_room != old_room) {
      draw_room(current_room);
      room_color = colors[rooms[current_room].color];
      draw_ball(ball_x, ball_y, room_color);
      uint8_t thin_walls = rooms[current_room].pf_control;

      if (thin_walls & 3) draw_thin_walls(thin_walls & 1);

      draw_objects();

      if (current_room == 0x11) {
        //draw_object(key_x, key_y, room_color, back_color, key);
        draw_portcullis(16);
      }
    }

    if (ball_x != old_ball_x || ball_y != old_ball_y) {
      draw_ball(old_ball_x, old_ball_y, back_color);
      draw_ball(ball_x, ball_y, room_color);
    }
  }
}

