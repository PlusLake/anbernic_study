#include <stdint.h>
#include "images.c"

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef volatile u8    vu8;
typedef volatile u16   vu16;
typedef volatile u32   vu32;

#define WIDTH    240
#define HEIGHT   160
#define VRAM     ((u16*) 0x06000000)

#define FRAME_SEL_BIT     0x0010
#define BG2_ENABLE        0x0400
#define MODE_BITMAP       0x0003
#define REGISTER_BASE     0x04000000
#define REGISTER_VERTICAL *(vu16*)(REGISTER_BASE + 0x0006)
#define REGISTER_KEY      *(vu16*)(REGISTER_BASE + 0x0130)

#define KEY_A      0x0001
#define KEY_B      0x0002
#define KEY_SELECT 0x0004
#define KEY_START  0x0008
#define KEY_RIGHT  0x0010
#define KEY_LEFT   0x0020
#define KEY_UP     0x0040
#define KEY_DOWN   0x0080
#define KEY_R      0x0100
#define KEY_L      0x0200
#define KEY_MASK   0x03FF
#define isKeyDown(key) (~(REGISTER_KEY) & key)
#define arraySize(array) (sizeof(array) / sizeof((array)[0]))

/*
void vsync() {
	while(REGISTER_VERTICAL >= 160);
	while(REGISTER_VERTICAL < 160);
}

void simpleLines() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == y)
                VRAM[y * WIDTH + x] = 31 << 5;
            if (!x || !y || x == WIDTH - 1 || y == HEIGHT -1)
                VRAM[y * WIDTH + x] = 31;
        }
    }
}
*/

void hifumi(u32 type) {
    const u32* HIFUMI[] = { HIFUMI_0, HIFUMI_1, HIFUMI_2, HIFUMI_3 };
    u32* vram32 = (u32*) VRAM;
    for (int i = 0; i < WIDTH * HEIGHT / 2; i++)
        vram32[i] = HIFUMI[type % arraySize(HIFUMI)][i];
}

int main(void) {
    static volatile u16 * const reg_disp_ctl = (void*) 0x04000000;
    *reg_disp_ctl = 3 | BG2_ENABLE;
    *reg_disp_ctl &= ~FRAME_SEL_BIT;
    u32 imageIndex = 0;
    while(1) {
        hifumi(imageIndex);
        while (!isKeyDown(KEY_A));
        imageIndex++;
    }
    return 0;
}
