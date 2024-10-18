#ifndef CHIP_8_PLATFORM_H
#define CHIP_8_PLATFORM_H

#include "SDL.h"

class Platform {
public:
    Platform(char const* title, int windowWidth, int windowHeight,
             int textureWidth, int textureHeight);
    ~Platform();

    void update(void const* buffer, int pitch);
    bool processInput(uint8_t* key);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif // CHIP_8_PLATFORM_H