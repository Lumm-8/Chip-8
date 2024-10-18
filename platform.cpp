#include "platform.h"

Platform::Platform(const char *title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, 100, 100, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, -1,
                                SDL_TEXTUREACCESS_STREAMING,
                                textureWidth, textureHeight);
}

Platform::~Platform() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(void const* buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform:: processInput(uint8_t* key) {
    bool quit = false;
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                quit = true;
            }
                break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: {
                        quit = true;
                    }
                        break;
                    case SDLK_x: {
                        key[0] = 1;
                    }
                        break;
                    case SDLK_1: {
                        key[1] = 1;
                    }
                        break;
                    case SDLK_2: {
                        key[2] = 1;
                    }
                        break;
                    case SDLK_3: {
                        key[3] = 1;
                    }
                        break;
                    case SDLK_q: {
                        key[4] = 1;
                    }
                        break;
                    case SDLK_w: {
                        key[5] = 1;
                    }
                        break;
                    case SDLK_e: {
                        key[6] = 1;
                    }
                        break;
                    case SDLK_a: {
                        key[7] = 1;
                    }
                        break;
                    case SDLK_s: {
                        key[8] = 1;
                    }
                        break;
                    case SDLK_d: {
                        key[9] = 1;
                    }
                        break;
                    case SDLK_z: {
                        key[10] = 1;
                    }
                        break;
                    case SDLK_c: {
                        key[11] = 1;
                    }
                        break;
                    case SDLK_4: {
                        key[12] = 1;
                    }
                        break;
                    case SDLK_r: {
                        key[13] = 1;
                    }
                        break;
                    case SDLK_f: {
                        key[14] = 1;
                    }
                        break;
                    case SDLK_v: {
                        key[15] = 1;
                    }
                        break;
                }
            }
                break;

            case SDL_KEYUP: {
                switch (event.key.keysym.sym) {
                    case SDLK_x: {
                        key[0] = 0;
                    }
                        break;
                    case SDLK_1: {
                        key[1] = 0;
                    }
                        break;
                    case SDLK_2: {
                        key[2] = 0;
                    }
                        break;
                    case SDLK_3: {
                        key[3] = 0;
                    }
                        break;
                    case SDLK_q: {
                        key[4] = 0;
                    }
                        break;
                    case SDLK_w: {
                        key[5] = 0;
                    }
                        break;
                    case SDLK_e: {
                        key[6] = 0;
                    }
                        break;
                    case SDLK_a: {
                        key[7] = 0;
                    }
                        break;
                    case SDLK_s: {
                        key[8] = 0;
                    }
                        break;
                    case SDLK_d: {
                        key[9] = 0;
                    }
                        break;
                    case SDLK_z: {
                        key[10] = 0;
                    }
                        break;
                    case SDLK_c: {
                        key[11] = 0;
                    }
                        break;
                    case SDLK_4: {
                        key[12] = 0;
                    }
                        break;
                    case SDLK_r: {
                        key[13] = 0;
                    }
                        break;
                    case SDLK_f: {
                        key[14] = 0;
                    }
                        break;
                    case SDLK_v: {
                        key[15] = 0;
                    } break;
                }
            }
             break;
        }
    }

    return quit;
}