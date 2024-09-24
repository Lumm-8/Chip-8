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
const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

class Chip8 {
    public:

    uint8_t registers[16];
    uint8_t memory[4096]; 
    uint16_t index{};
    uint16_t pc{};

    // 16bit stack
    uint8_t sp;
    uint16_t stack[16];
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

    typedef void (Chip8::*Chip8Func) ();

    Chip8Func table[0xF + 1];
    Chip8Func table0[0xF];
    Chip8Func table8[0xF];
    Chip8Func tableE[0xF];
    Chip8Func tableF[0x65 + 1];

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

        table[0] = Chip8::Table0;
        table[1] = Chip8::op_1nnn;
        table[2] = Chip8::op_2nnn;
        table[3] = Chip8::op_3xkk;
        table[4] = Chip8::op_4xkk;
        table[5] = Chip8::op_5xy0;
        table[6] = Chip8::op_6xkk;
        table[7] = Chip8::op_7xkk;
        table[8] = Chip8::Table8;
        table[9] = Chip8::op_9xy0;
        table[0xA] = Chip8::op_Annn;
        table[0xB] = Chip8::op_Bnnn;
        table[0xC] = Chip8::op_Cxkk;
        table[0xD] = Chip8::op_Dxyn;
        table[0xE] = Chip8::TableE;
        table[0xF] = Chip8::TableF;

        for (int i = 0; i < 0xF; i++) {
            table0[i] = Chip8::op_NULL;
            table8[i] = Chip8::op_NULL;
            tableE[i] = Chip8::op_NULL;
        }

        table0[0x0] = Chip8::op_00E0;
        table0[0xE] = Chip8::op_00EE;

        table8[0x0] = Chip8::op_8xy0;
        table8[0x1] = Chip8::op_8xy1;
        table8[0x2] = Chip8::op_8xy2;
        table8[0x3] = Chip8::op_8xy3;
        table8[0x4] = Chip8::op_8xy4;
        table8[0x5] = Chip8::op_8xy5;
        table8[0x6] = Chip8::op_8xy6;
        table8[0x7] = Chip8::op_8xy7;
        table8[0xE] = Chip8::op_8xyE;

        tableE[0xE] = Chip8::op_Ex9E;
        tableE[0x1] = Chip8::op_ExA1;

        for (int i = 0; i <= 0x65; i++) {
            tableF[i] = Chip8::op_NULL;
        }

        tableF[0x07] = Chip8::op_Fx07;
        tableF[0x0A] = Chip8::op_Fx0A;
        tableF[0x15] = Chip8::op_Fx15;
        tableF[0x18] = Chip8::op_Fx18;
        tableF[0x1E] = Chip8::op_Fx1E;
        tableF[0x29] = Chip8::op_Fx29;
        tableF[0x33] = Chip8::op_Fx33;
        tableF[0x55] = Chip8::op_Fx55;
        tableF[0x65] = Chip8::op_Fx65;
    }

    void LoadRom(char const* fileName); 

    // instruction functions. opinions
    
    // cls clear the display.
    void op_00E0(); 
    // ret return from subroutine.
    void op_00EE();
    // 1nnn: Jp addr, jump to address nnn.
    void op_1nnn();
    // 2nnn: call addr, call subroutine at nnn.
    void op_2nnn();
    // 3xkk: se vx, byte, skip next instruction if Vx == kk.
    // compare the value of register Vx with the immediate value kk.
    void op_3xkk();
    // 4xkk: SNE Vx, byte, skip next instruction if Vx != kk.
    void op_4xkk();
    // 5xy0: SE Vx, Vy, skip next instruction if Vx == Vy.
    // Vx and Vy are two registers, V meaning the value of register.
    void op_5xy0();
    // 6xkk: LD Vx, byte, set Vx = kk.
    void op_6xkk();
    // 7xkk: ADD Vx, byte, set Vx = Vx + kk.
    void op_7xkk();
    // 8xy0: LD Vx, Vy, set Vx = Vy.
    void op_8xy0();
    // 8xy1: OR Vx, Vy, set Vx = Vx OR Vy.
    void op_8xy1();
    // 8xy2: AND Vx, Vy, set Vx = Vx AND Vy.
    void op_8xy2();
    // 8xy3: XOR Vx, Vy, set Vx = Vx XOR Vy.
    void op_8xy3();
    // 8xy4: ADD Vx, Vy, set Vx = Vx + Vy, set VF = carry.
    void op_8xy4();
    // 8xy5: SUB Vx, Vy, set Vx = Vx - Vy, set VF = NOT borrow.
    void op_8xy5();
    // 8xy6: SHR Vx 1
    void op_8xy6();
    // 8xy7: SUBN Vx, Vy, set Vx = Vy - Vx, set VF = NOT borrow.
    void op_8xy7();
    // 8xyE: SHL Vx {, Vy}, if the most significant bit of Vx is 1,
    // VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
    void op_8xyE();
    // 9xy0: SNE Vx, Vy, skip next instruction if Vx != Vy.
    void op_9xy0();
    // Annn: LD I, addr, set I = nnn.
    void op_Annn();
    //  Bnnn: JP V0, addr, jump to location nnn + V0.
    void op_Bnnn();
    // Cxkk: RND Vx, byte, set Vx = random byte AND kk.
    void op_Cxkk();
    // Dxyn: DRW Vx, Vy, nibble,
    // display n-byte sprite starting at memory location I at(Vx, Vy)
    void op_Dxyn();
    // Ex9E: SKP Vx,
    // skip next instruction if key with the value of Vx is pressed.
    void op_Ex9E();
    // ExA1: SKNP Vx
    // skip next instruction if key with the value of Vx is not pressed.
    void op_ExA1();
    // Fx07: LD Vx, DT, set Vx = delay timer value.
    void op_Fx07();
    // Fx0A: LD Vx, K, 
    // wait for  a key press, store the value of the key in Vx.
    void op_Fx0A();
    // Fx15: LD DT, Vx, set delay timer = Vx.
    void op_Fx15();
    // Fx18: LD ST, Vx, set sound timer = Vx
    void op_Fx18();
    // Fx1E: ADD I, Vx, set I = I + Vx.
    void op_Fx1E();
    // Fx29: LD F, Vx, set I = Location of sprite for digit Vx.
    void op_Fx29();
    // Fx33: LD B, Vx,
    // store BCD representation of Vx in memory locations I, I+1, and I+2.
    void op_Fx33();
    // Fx55: LD [I], Vx,
    // store registers V0 through Vx in memory starting at location I.
    void op_Fx55();
    // Fx65: LD Vx, [I],
    // read registers V0 through Vx from memory starting at location I.
    void op_Fx65();

    void Table0();
    void Table8();
    void TableE();
    void TableF();
    void op_NULL();
};


#endif //CHIP_8_CHIP8_H
