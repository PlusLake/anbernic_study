#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.h"

void logs(const char *format, ...) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%S", timeinfo);
    printf("[%s] ", time_str);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}

#define SCREEN_COLS 45
#define SCREEN_ROWS 26
#define FONT_WIDTH 14
#define FONT_HEIGHT 18
#define FONT_COLS 18

void menu_reboot() {
    system("reboot");
}
void menu_shutdown() {
    system("poweroff");
}
typedef struct {
    const char* text;
    void (*callback)(void);
} MenuItem;

MenuItem menus[] = {
    {"Reboot", menu_reboot},
    {"Shutdown", menu_shutdown},
};

void drawText(SDL_Renderer *renderer, SDL_Texture *ascii, const char *text, int x, int y) {
    for (int i = 0; text[i]; i++) {
        char character = text[i] - 32;
        int fontY = character / FONT_COLS * FONT_HEIGHT;
        int fontX = character % FONT_COLS * FONT_WIDTH;
        SDL_Rect sourceRect = { fontX, fontY, FONT_WIDTH, FONT_HEIGHT };
        SDL_Rect targetRect = { x + (i * FONT_WIDTH), y, FONT_WIDTH, FONT_HEIGHT };
        SDL_RenderCopy(renderer, ascii, &sourceRect, &targetRect);
    }
}

void loop(SDL_Renderer *renderer, SDL_Texture *ascii) {
    bool running = true;
    SDL_Event event;
    const int MAX_MENU = 2;
    int currentMenu = 0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_JOYBUTTONDOWN:
                    switch (event.jbutton.button) {
                        case BUTTON_A:
                            menus[currentMenu].callback();
                            running = false;
                            break;
                    }
                    break;
                case SDL_JOYHATMOTION:
                    switch (event.jhat.value) {
                        case BUTTON_UP:
                            currentMenu = (currentMenu - 1 + MAX_MENU) % MAX_MENU;
                            break;
                        case BUTTON_DOWN:
                            currentMenu = (currentMenu + 1) % MAX_MENU;
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        int menuLength = sizeof(menus) / sizeof(menus[0]);
        for (int y = 0; y < menuLength; y++) {
            if (y == currentMenu) {
                drawText(renderer, ascii, ">", 0, y * FONT_HEIGHT);
            }
            drawText(renderer, ascii, menus[y].text, 2 * FONT_WIDTH, y * FONT_HEIGHT);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawText(renderer, ascii, "Good bye!", 0, 0);
    SDL_RenderPresent(renderer);
}

SDL_Texture* loadAscii(SDL_Renderer *renderer) {
    SDL_Surface *surface = SDL_LoadBMP("/opt/muos/bin/ascii.bmp");
    if (!surface) {
        logs("Failed to load image: %s\n", SDL_GetError());
        return NULL;
    }
    Uint32 colorkey = SDL_MapRGB(surface->format, 0, 0, 0);
    SDL_SetColorKey(surface, SDL_TRUE, colorkey);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main(void) {
    logs("Starting program...\n");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        logs("Failed to initialize SDL! Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Hello MuOS",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_FULLSCREEN
    );

    if (window == NULL) {
        logs("Failed to create SDL Window! Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        logs("Renderer could not be created! Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Joystick *joystick = NULL;
    if (SDL_NumJoysticks() > 0) {
        joystick = SDL_JoystickOpen(0);
        if (joystick) {
            logs("Successfully opened joystick: %s\n", SDL_JoystickName(joystick));
        } else {
            logs("Found a joystick but failed to open!\n");
            return 1;
        }
    } else {
        logs("No joysticks detected!");
        return 1;
    }
    SDL_Texture *ascii = loadAscii(renderer);
    if (!ascii) {
        return 1;
    }
    loop(renderer, ascii);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    logs("Ending program...\n");
    return 0;
}
