#include <iostream>
#include "Chip8.h"
#include "platform.h"
#include <chrono>
//#include "SDL_main.h"

//#define SDL_MAIN_HANDLED

int main(int argc, char** argv) {

    //std::cout << " ---------" << std::endl;
    if (argc < 4) {
        std::cerr << "Usage:" << argv[0] << " <Scale><Delay><ROM>" << std::endl;

    }

    int scale = std::stoi(argv[1]);
    int delay = std::stoi(argv[2]);
    char const* rom = argv[3];

    //std::cout << "Scale: " << scale << " Delay: " << delay << " ROM: " << rom << std::endl;
    Platform platform("Chip-8 emulator", VIDEO_WIDTH * scale, VIDEO_WIDTH * scale, VIDEO_WIDTH, VIDEO_WIDTH);
    Chip8 chip8;
    chip8.LoadRom(rom);

    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while (!quit) {
        quit = platform.processInput(chip8.keypad);

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastCycleTime).count();

        if (dt > delay) {
            lastCycleTime = currentTime;
            chip8.Cycle();
            platform.update(chip8.video, videoPitch);
        }
    }

    return 0;
}
