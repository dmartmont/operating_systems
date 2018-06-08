#include <bitset>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "interprete.h"
#include "util.h"

Interprete::Interprete(char* filename, char* memName){
    init(filename);

    int* configMem = open_shared_memory((char*)"config", 28);

    // Size of shared memory
    int memSize = *configMem;
    configMem += 1;

    std::vector<Segment> segments;

    for (int i = 0; i < 6; ++i) {
        int segment = *(configMem + i);

        struct Segment segmentObj;

        segmentObj.base = (segment >> 16) << 2;
        int limit = ((segment << 16) >> 16);

        if(i == 2 || i == 4) {
            limit = limit + (4 - limit % 4);
        } else {
            limit <<= 2;
        }

        segmentObj.limit = limit;

        segments.push_back(segmentObj); 
    }

    memoria = new Memory(memName, memSize, segments);
}

void Interprete::init(char* filename) {
    char* inputData = open_file(filename);
    size_t size = strlen(inputData);

    size_t i = 0;
    while (i < size) {
        char opcode = (inputData[i]);

        opcode = (opcode >> 4) & 0x0F;

        // 8 bits instructions
        if(opcode == (char)0x0F || opcode == (char)0x0E) {
            std::bitset<8> value = std::bitset<8>(inputData[i]);
            instructions.push_back(value.to_string());
            ++i;
        } 
        // 32 bits intructions
        else if(opcode == (char)0x03 || opcode == (char)0x08 || 
                opcode == (char)0x09 || opcode == (char)0x0B || 
                opcode == (char)0x0C) {

            std::string instruction ="";
            
            for (int j = i; j < i + 4; ++j) {
                instruction.append(std::bitset<8>(inputData[j]).to_string());
            }

            instructions.push_back(instruction);
            i += 4;
        }
        // 64 bits instructions
        else if(opcode == (char)0x00 || opcode == (char)0x01 || 
                opcode == (char)0x02 || opcode == (char)0x04 || 
                opcode == (char)0x05 || opcode == (char)0x06 ||
                opcode == (char)0x07 || opcode == (char)0x0A || 
                opcode == (char)0x0D) {

            std::string instruction ="";
            
            for (int j = i; j < i + 8; ++j) {
                instruction.append(std::bitset<8>(inputData[j]).to_string());
            }

            instructions.push_back(instruction);
            i += 8;
        }
    }
}

void Interprete::run() {
    for(std::vector<std::string>::size_type i = 0; i <= instructions.size(); ++i) {
        std::string instruction = instructions[i];
        
        // 8 bits instructions
        if(instruction.substr(0, 4) == "1110") {
            haltInst();
        }
        else if(instruction.substr(0, 4) == "1111") {
            breakInst();
        }
        // 32 bits instructions
        else if(instruction.substr(0, 4) == "0011") {
            memrefToPc(instruction);
        }
        else if(instruction.substr(0, 4) == "1000") {
            readInt(instruction);
        }
        else if(instruction.substr(0, 4) == "1001") {
            writeInt(instruction);
        }
        else if(instruction.substr(0, 4) == "1011") {
            writeStr(instruction);
        }
        else if(instruction.substr(0, 4) == "1100") {
            goTo(instruction);
        }
        // 64 bits instructions
        else if(instruction.substr(0, 4) == "0000") {
            moveNum(instruction);
        }
        else if(instruction.substr(0, 4) == "0001") {
            moveStr(instruction);
        }
        else if(instruction.substr(0, 4) == "0010") {
            saveNumPlusPC(instruction);
        }
        else if(instruction.substr(0, 4) == "0100") {
            moveValueInMemory(instruction);
        }
        else if(instruction.substr(0, 4) == "0101") {
            arithmeticOp(instruction);
        }
        else if(instruction.substr(0, 4) == "0110") {
            moveValueInMemoryInteger(instruction);
        }
        else if(instruction.substr(0, 4) == "0111") {
            saveDisplaced(instruction);
        }
        else if(instruction.substr(0, 4) == "1010") {
            readStr(instruction);            
        }
        else if(instruction.substr(0, 4) == "1101") {
            conditionalJump(instruction);
        }
    }


}

// 8 bit instructions
void Interprete::haltInst() {
}

void Interprete::breakInst() {
    break;
}

// 32 bits instructions
void Interprete::memrefToPc(std::string instruction) {
    const char* memref = instruction.substr(4, 15).c_str();
}

void Interprete::readInt(std::string instruction) {
    const char* memref = instruction.substr(4, 15).c_str();
}

void Interprete::writeInt(std::string instruction) {
    const char* memref = instruction.substr(4, 15).c_str();
}

void Interprete::writeStr(std::string instruction) {
    const char* memref = instruction.substr(4, 15).c_str();
}

void Interprete::goTo(std::string instruction) {
    const char* address = instruction.substr(4, 15).c_str();
}

// 64 bits instructions

void Interprete::moveNum(std::string instruction) {
    const char* memref = (instruction.substr(4, 15)).c_str();
    const char* integer = (instruction.substr(19, 15)).c_str();
    int value = memoria->readInt(LITNUM, (char*)memref);
    memoria->writeInt(DATANUM, (char*)integer, value);
}

void Interprete::moveStr(std::string instruction) {
    const char*  memref = instruction.substr(4, 15).c_str();
    const char*  strRef = instruction.substr(19, 15).c_str();
    char* value = memoria->readChar(LITSTR, (char*)memref);
    memoria->writeChar(DATASTR, (char*)strRef, value);
}

void Interprete::saveNumPlusPC(std::string instruction) {
    const char*  memref = instruction.substr(4, 15).c_str();
    const char*  integer = instruction.substr(19, 15).c_str();
}

void Interprete::moveValueInMemory(std::string instruction) {
    const char* type         = instruction.substr(4,1).c_str();
    const char*  memrefdst    = instruction.substr(5, 15).c_str();
    const char*  memrefsrc    = instruction.substr(20, 15).c_str();
}

void Interprete::arithmeticOp(std::string instruction) {
    const char*  operation    = instruction.substr(4, 3).c_str();
    const char*  flag         = instruction.substr(7, 1).c_str();
    const char*  memrefdst    = instruction.substr(8, 15).c_str();
    const char*  memrefop1    = instruction.substr(24, 15).c_str();
    const char*  memrefop2    = instruction.substr(39, 15).c_str();
}

void Interprete::moveValueInMemoryInteger(std::string instruction) {
    const char*  type         = instruction.substr(4,1).c_str();
    const char*  memrefdst    = instruction.substr(5, 15).c_str();
    const char*  memrefsrc    = instruction.substr(20, 15).c_str();
    const char*  integer      = instruction.substr(35, 15).c_str();
}

void Interprete::saveDisplaced(std::string instruction) {
    const char* type         = instruction.substr(4,1).c_str();
    const char* memrefdst    = instruction.substr(5, 15).c_str();
    const char* integer      = instruction.substr(20, 15).c_str();
    const char* memrefsrc    = instruction.substr(35, 15).c_str();
}

void Interprete::readStr(std::string instruction) {
    const char* memrefdst    = instruction.substr(4, 15).c_str();
    const char* memrefsize   = instruction.substr(19, 15).c_str();                
}

void Interprete::conditionalJump(std::string instruction) {
    const char* operation    = instruction.substr(4, 3).c_str();
    const char* memref1    = instruction.substr(7, 15).c_str();
    const char* memref2    = instruction.substr(22, 15).c_str();
    const char* integer      = instruction.substr(37, 15).c_str();
}