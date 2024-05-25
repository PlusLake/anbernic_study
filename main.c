#include <stdint.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#define WIDTH    240
#define HEIGHT   160
#define VRAM     ((u16*) 0x06000000)

#define FRAME_SEL_BIT   0x0010
#define BG2_ENABLE      0x0400
#define MODE_BITMAP     0x0003

int main(void) {
    static volatile u16 * const reg_disp_ctl = (void*) 0x04000000;
    *reg_disp_ctl = 3 | BG2_ENABLE;
    *reg_disp_ctl &= ~FRAME_SEL_BIT;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == y) {
                VRAM[y * WIDTH + x] = 31 << 5;
	        }
            if (!x || !y || x == WIDTH - 1 || y == HEIGHT -1) {
                VRAM[y * WIDTH + x] = 31;
            }
        }
    }

    while(1);
    return 0;
}
