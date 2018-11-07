#include <stdint.h>
#include <stdbool.h>
#include <uart/uart.h>
#include <ili9341/ili9341.h>
#include <delay/delay.h>
#include <button/button.h>

#define reg_uart_clkdiv (*(volatile uint32_t*)0x02000004)
#define reg_buttons (*(volatile uint32_t*)0x03000004)

#define reg_uart_data (*(volatile uint32_t*)0x02000008)

uint16_t colors[] = {0xF81F, 0xFFFF};
uint16_t back_color;

uint32_t room0[] = {

       0xFFFFF,          //XXXXXXXXXXXXXXXXXXXXRRRRRRRRRRRRRRRRRRRR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xC0000,          //XX                                    RR
       0xFFFF0           //XXXXXXXXXXXXXXXXXX    RRRRRRRRRRRRRRRRRR
}; 

typedef struct Rooms {
  uint32_t *data;
  uint8_t color;
  uint8_t pf_control;
  uint8_t above, left, down, right;
} Rooms;

Rooms rooms[] = {
  {room0, 0 , 0x00, 0, 0, 0, 0}
};


void printRoom(int r) {
  for(int y=0; y<7; y++) {
    uint32_t bit = 0x80000;
    uint32_t pf = rooms[r].data[y];
    for(int x=0; x<20; x++) {
      for(int i = 0; i< ((y == 0 || y == 6) ? 16 : 32);i++) {
        uint16_t c = colors[rooms[i].color];
        int yy =  24 + (y == 0 ? i : 16 + i + ((y-1) * 32));
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

void main() {
    reg_uart_clkdiv = 139;

    lcd_init();

    lcd_clear_screen(0);

    back_color = colors[1];

    printRoom(0);

    while(true) {}
}

