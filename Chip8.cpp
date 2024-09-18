//
// Created by 29808 on 2024/9/9.
//


#include <fstream>
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