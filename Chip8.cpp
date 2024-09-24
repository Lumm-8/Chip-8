//
// Created by 29808 on 2024/9/9.
//


#include <fstream>
#include <cstring>
#include "Chip8.h"

void Chip8::LoadRom(char const* fileName) {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    
    if (file.is_open()) {
        // tellg: get size of files
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i < size; i++) {
            memory[START_ADDRESS + i] = buffer[i];
        }
        
        delete[] buffer;
    }
    
}

void Chip8::op_00E0() {
   memset(video, 0, sizeof(video)); 
}

void Chip8::op_00EE() {
   pc = stack[--sp]; 
}

void Chip8::op_1nnn() {
    uint16_t address = opcode & 0x0FFFU;
    pc = address;
}

void Chip8::op_2nnn() {
    uint16_t address = opcode & 0x0FFFU;
    
    // need to hold the next instruction address of call
    stack[sp++] = pc; 
    pc = address;
}

void Chip8::op_3xkk() { 
    uint8_t x = (opcode & 0x0F00U) >> 8u;
    uint16_t kk = opcode & 0x00FFU;

    // now pc point to the next instruction of 3xkk,
    // so only need to add 2 to skip it
    if (registers[x] == kk) {
        pc += 2;
    }
}

void Chip8::op_4xkk() { 
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint16_t kk = opcode & 0x00FFU;

    if (registers[x] != kk) {
        pc += 2;
    }
}

void Chip8::op_5xy0() { 
    uint8_t x = (opcode & 0x0F00U) >> 8u;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    if (registers[x] == registers[y]) {
        pc += 2;
    }
}

void Chip8::op_6xkk() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint16_t kk = opcode & 0x00FFU;

    registers[x] = kk;
}

void Chip8::op_7xkk() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint16_t kk = opcode & 0x00FFU;

    registers[x] += kk;
}

void Chip8::op_8xy0() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    registers[x] = registers[y];
}

void Chip8::op_8xy1() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;
    
    registers[x] |= registers[y];
}

void Chip8::op_8xy2() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    registers[x] &= registers[y];
}

void Chip8::op_8xy3() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    registers[x] ^= registers[y];
}

void Chip8::op_8xy4() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    uint16_t sum = registers[x] + registers[y];

    if (sum > 255U) { 
        registers[0xF] = 1;
    }
    else {
        registers[0xF] = 0;
    }

    registers[x] = sum & 0xFFU;
}

void Chip8::op_8xy5() { 
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    if (registers[x] > registers[y]) {
        registers[0xF] = 1;
    }
    else {
        registers[0xF] = 0;
    }

    registers[x] = registers[x] - registers[y];
}

void Chip8::op_8xy6() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    registers[0xF] = registers[x] & 0x1U;
    registers[x] >>= 1;
}

void Chip8::op_8xy7() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    registers[0xF] = registers[y] > registers[x];

    registers[x] = registers[y] - registers[x];
}

void Chip8::op_8xyE() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    registers[0xF] = registers[x] & 0x80U;
    
    registers[x] <<= 1;
}

void Chip8::op_9xy0() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    if (registers[x] != registers[y]) {
        pc += 2;
    }
}

void Chip8::op_Annn() {
    uint16_t address = opcode & 0x0FFFU;
    index = address;
}

void Chip8::op_Bnnn() {
    uint16_t address = opcode & 0x0FFFU;
    uint8_t v0 = registers[0];

    pc = address + v0;
}

void Chip8::op_Cxkk() {
    uint16_t kk = opcode & 0x00FFU;
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    registers[x] = randByte(randGen) & kk;
}


void Chip8::op_Dxyn() {
    uint8_t x = (opcode & 0x0F00u) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;

    uint8_t n = opcode & 0x000FU;

    uint8_t xPos = registers[x] % VIDEO_WIDTH;
    uint8_t yPos = registers[y] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for (unsigned int i = 0; i < n; i++) {
        uint8_t spriteByte = memory[index + i];

        for (unsigned int j = 0; j < 8; j++) {
           uint8_t spritePixel = spriteByte & (0x80U >> j);
           uint32_t *screenPixel = &video[(yPos + i) * VIDEO_WIDTH + (xPos + j)];

           if (spritePixel) {
               if (*screenPixel & 0xFFFFFFFFU) {
                   registers[0xF] = 1;
               }

               *screenPixel ^= 0xFFFFFFFFU;
           } 
        }
    }
}

void Chip8::op_Ex9E() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t key = registers[x];

    if (keypad[key]) {
        pc += 2;
    }
}

void Chip8::op_ExA1() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    if (!keypad[registers[x]]) {
        pc += 2;
    }
}

void Chip8::op_Fx07() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    registers[x] = delay_timer;
}


void Chip8::op_Fx0A() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    if (keypad[0]) {
        registers[x] = 0;
    }
    else if (keypad[1]) {
        registers[x] = 1;
    }
    else if (keypad[2]) {
        registers[x] = 2;
    }
    else if (keypad[3]) {
        registers[x] = 3;
    }
    else if (keypad[4]) {
        registers[x] = 4;
    }
    else if (keypad[5]) {
        registers[x] = 5;
    }
    else if (keypad[6]) {
        registers[x] = 6;
    }
    else if (keypad[7]) {
        registers[x] = 7;
    }
    else if (keypad[8]) {
        registers[x] = 8;
    }
    else if (keypad[9]) {
        registers[x] = 9;
    }
    else if (keypad[0xA]) {
        registers[x] = 0xA;
    }
    else if (keypad[0xB]) {
        registers[x] = 0xB;
    }
    else if (keypad[0xC]) {
        registers[x] = 0xC;
    }
    else if (keypad[0xD]) {
        registers[x] = 0xD;
    }
    else if (keypad[0xE]) {
        registers[x] = 0xE;
    }
    else if (keypad[0xF]) {
        registers[x] = 0xF;
    }
}

void Chip8::op_Fx15() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    delay_timer = registers[x];
}

void Chip8::op_Fx18() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    sound_timer = registers[x];
}

void Chip8::op_Fx1E() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    index = index + registers[x];
}

void Chip8::op_Fx29() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    index = FONTSET_START_ADDRESS + 5 * registers[x];
}

void Chip8::op_Fx33() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t value = registers[x];

    memory[index] = value / 100;
    memory[index + 1] = (value % 100) / 10;
    memory[index + 2] = value % 10;
}

void Chip8::op_Fx55() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    for (uint8_t i = 0; i <= x; i++) {
        memory[index + i] = registers[i];
    }
}

void Chip8::op_Fx65() {
    uint8_t x = (opcode & 0x0F00U) >> 8U;

    for (uint8_t i = 0; i <= x; i++) {
        registers[i] = memory[index + i];
    }
}

void Chip8::Table0() {
    ((*this).*(table0[opcode & 0x000FU]))();
}

void Chip8::Table8() {
    ((*this).*(table8[(opcode & 0x000FU)]))();
}

void Chip8::TableE() {
    ((*this).*(tableE[(opcode & 0x000FU)]))();
}

void Chip8::TableF() {
    (((*this).*(tableF[(opcode & 0x00FFU)])))();
}

void Chip8::op_NULL() {
    // do nothing
}