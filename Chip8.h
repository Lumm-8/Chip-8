//
// Created by 29808 on 2024/9/9.
//

#ifndef CHIP_8_CHIP8_H
#define CHIP_8_CHIP8_H

#include <cstdint>
#include <chrono>
#include <random>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_SIZE  = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

class Chip8 {
    public:

    uint8_t regsiter[16];
    uint8_t memory[4096]; 
    uint16_t index{};
    uint16_t pc{};

    // 16bit stack
    uint8_t sp;
    uint8_t delay_timer{};
    uint8_t sound_timer{};
    uint8_t keypad[16];
    uint32_t video[64*32];
    uint16_t opcode;

    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    uint8_t font_set[FONTSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    /// @brief 
    /// @return 
    Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
        // initialize pc
        pc = START_ADDRESS;

        for (int i = 0; i < FONTSET_SIZE; i++) {
            memory[FONTSET_START_ADDRESS + i] = font_set[i];
        }

        // intialize RNG
        randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
    }

    void LoadRom(char const* fileName); 
    
};


#endif //CHIP_8_CHIP8_H
