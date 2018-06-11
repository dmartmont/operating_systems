#include <bitset>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>

#include "interprete.h"
#include "util.h"

Interprete::Interprete(char* filename, char* memName){
    program_counter = 0;
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
    std::ifstream is (filename, std::ios::binary | std::ios::in | std::ios::ate);

    size_t size;
    char* inputData;

    if (is) {
        size = is.tellg();
        inputData = new char[size];

        is.seekg(0, std::ios::beg);
        is.read(inputData, size);

        if (!is) std::cout << "error reading file, only read " << is.gcount() << std::endl;

        is.close();

        inputData[size] = '\0';
    }

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
    for(std::vector<std::string>::size_type i = 0; i < instructions.size(); ++i) {

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

        ++program_counter;
    }


}

// 8 bit instructions
void Interprete::haltInst() {
}

void Interprete::breakInst() {
}

// 32 bits instructions
void Interprete::memrefToPc(std::string instruction) {
    char* memref = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());
    program_counter = memoria->readInt(LITNUM, memref);

    std::cout << "2.2.4 memrefToPC: memref -> " << memref << std::endl;    
}

void Interprete::readInt(std::string instruction) {
    char* memref = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());

    int value;
    std::cin >> value;
    memoria->writeInt(DATANUM, memref, value);

    std::cout << "2.2.9 ReadInt: memref -> " << memref << std::endl;    
}

void Interprete::writeInt(std::string instruction) {
    char* memref = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());
    int res = memoria->readInt(DATANUM, memref);

    std::cout << "2.2.10 WriteInt: memref -> " << memref << std::endl;
    std::cout << "2.2.10 WriteInt result: " << std::dec << res << std::endl;
}

void Interprete::writeStr(std::string instruction) {
    char* memref = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());

    int memrefVal = strtol(memref, nullptr, 2);
    char value;
    std::string result;
    int i = 0;
    int j = 0;

    do {
        int newMemRef = memrefVal + (((j * 4) + (4 - (i % 4))) - 1);
        value = memoria->readChar(DATASTR, newMemRef);

        result.push_back(value);

        i++;
        if (i % 4 == 0) j++;
    } while (value);

    std::cout << "2.2.12 WriteStr: memref -> " << memref << std::endl;
    std::cout << "2.2.12 WriteStr result: " << result << std::endl;
}

void Interprete::goTo(std::string instruction) {
    char* memref = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());

    program_counter = memoria->readInt(LITNUM, memref);

    std::cout << "2.2.13 goTo: memref -> " << memref << std::endl;
}

// 64 bits instructions

void Interprete::moveNum(std::string instruction) {
    char* memref = new char[15];
    char* integer = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());
    std::strcpy(integer, instruction.substr(19, 15).c_str());

    std::cout << "2.2.1 MoveNum: memref memrefinteger -> " << memref << " " << integer << std::endl;

    int value = memoria->readInt(LITNUM, integer);
    memoria->writeInt(DATANUM, memref, value);
}

void Interprete::moveStr(std::string instruction) {
    char* memref = new char[15];
    char* strRef = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());
    std::strcpy(strRef, instruction.substr(19, 15).c_str());

    std::cout << "2.2.2 MoveStr: memref memrefstring -> " << memref << " " << strRef << std::endl;
    
    int memRefVal = strtol(memref, nullptr, 2);
    int strRefVal = strtol(strRef, nullptr, 2);
    int i = 0;
    int j = 0;
    char value;
   
    do {
        int newMemRef = memRefVal + (((j * 4) + (4 - (i % 4))) - 1);
        int newStrRef = strRefVal + (((j * 4) + (4 - (i % 4))) - 1);

        value = memoria->readChar(LITSTR, newStrRef);
        memoria->writeChar(DATASTR, newMemRef, value);

        i++;
        if (i % 4 == 0) j++;
    } while(value);
    memoria->writeChar(DATASTR, strRefVal + i + 1, '\0');
}

void Interprete::saveNumPlusPC(std::string instruction) {
    char* memref = new char[15];
    char* integer = new char[15];
    std::strcpy(memref, instruction.substr(4, 15).c_str());
    std::strcpy(integer, instruction.substr(19, 15).c_str());
    int value = memoria->readInt(LITNUM, integer);
    value = program_counter + value;
    memoria->writeInt(DATANUM, memref, value);

    std::cout << "2.2.3 saveNumPlusPC: memref memrefinteger -> " << memref << " " << integer << std::endl;
}

void Interprete::moveValueInMemory(std::string instruction) {
    char* type = new char[1];
    char* memrefdst = new char[15];
    char* memrefsrc = new char[15];
    std::strcpy(type, instruction.substr(4, 1).c_str());
    std::strcpy(memrefdst, instruction.substr(5, 15).c_str());
    std::strcpy(memrefsrc, instruction.substr(20, 15).c_str());

    std::cout << "2.2.5 MoveNum: type memrefdst memrefsrc -> " << type << " " << memrefdst << " " << memrefsrc << std::endl;

    if(type == "1") {
        int value = memoria->readInt(DATANUM, memrefsrc);
        memoria->writeInt(DATANUM, memrefdst, value);
    } else {
        char charActual;
        int memSrcInt = strtol(memrefsrc, nullptr, 2);
        int memDstInt = strtol(memrefdst, nullptr, 2);
        int i = 0;
        int j = 0;
        do {
            int newSrcRef = memSrcInt + (((j * 4) + (4 - (i % 4))) - 1);
            int newDstRef = memDstInt + (((j * 4) + (4 - (i % 4))) - 1);

            charActual = memoria->readChar(DATASTR, newSrcRef);
            memoria->writeChar(DATASTR, newDstRef, charActual);

            i++;
            if (i % 4 == 0) j++;
        } while (charActual);
    }
    
}

void Interprete::arithmeticOp(std::string instruction) {
    char* operation = new char[3];
    char* flag = new char[1];
    char* memrefdst = new char[15];
    char* memrefop1 = new char[15];
    char* memrefop2 = new char[15];
    std::strcpy(operation, instruction.substr(4, 3).c_str());
    std::strcpy(flag, instruction.substr(7, 1).c_str());
    std::strcpy(memrefdst, instruction.substr(8, 15).c_str());
    std::strcpy(memrefop1, instruction.substr(23, 15).c_str());
    std::strcpy(memrefop2, instruction.substr(38, 15).c_str());

    std::cout << "2.2.6 ArithmeticOp: op type memrefdst memrefop1 memrefop2 -> " 
        << operation << " " 
        << flag << " " 
        << memrefdst << " " 
        << memrefop1 << " " 
        << memrefop2 << std::endl;

    if (flag) {
        int op1 = memoria->readInt(DATANUM, memrefop1);
        int op2 = memoria->readInt(DATANUM, memrefop2);
        int resultado;
        if(operation == "000") {
            resultado = op1 + op2;
        }
        else if(operation == "001") {
            resultado = op1 - op2;
        }
        else if(operation == "010") {
            resultado = op1 * op2;
        }
        else if(operation == "011") {
            resultado = op1 / op2;
        }
        else if(operation == "100") {
            resultado = op1 % op2;
        }
        memoria->writeInt(DATANUM, memrefdst, resultado);
    } else {
        char op1 = memoria->readChar(DATASTR, strtol(memrefop1, nullptr, 2));
        char op2 = memoria->readChar(DATASTR, strtol(memrefop2, nullptr, 2));
        char resultado;
        if(operation == "000") {
            resultado = op1 + op2;
        }
        else if(operation == "001") {
            resultado = op1 - op2;
        }
        else if(operation == "010") {
            resultado = op1 * op2;
        }
        else if(operation == "011") {
            resultado = op1 / op2;
        }
        else if(operation == "100") {
            resultado = op1 % op2;
        }
        memoria->writeChar(DATASTR, strtol(memrefdst, nullptr, 2), resultado);
    }
}

void Interprete::moveValueInMemoryInteger(std::string instruction) {
    char* type = new char[1];
    char* memrefdst = new char[15];
    char* memrefsrc = new char[15];
    char* integer = new char[15];
    std::strcpy(type, instruction.substr(4, 1).c_str());
    std::strcpy(memrefdst, instruction.substr(5, 15).c_str());
    std::strcpy(memrefsrc, instruction.substr(20, 15).c_str());
    std::strcpy(integer, instruction.substr(35, 15).c_str());

    std::cout << "2.2.7 moveValueInMemoryInteger: type memrefdst memrefsrc memrefinteger -> " 
        << type << " " 
        << memrefdst << " "
        << memrefsrc << " " 
        << integer << std::endl;

    int offset = memoria->readInt(LITNUM, integer);
    if(type == "1") {
        int value = memoria->readInt(DATANUM, (memrefsrc + offset));
        memoria->writeInt(DATANUM, memrefdst, value);
    } else {
        char value = memoria->readChar(DATASTR, strtol(memrefsrc, nullptr, 2) + offset);
        memoria->writeChar(DATASTR, strtol(memrefdst, nullptr, 2), value);
    }
}

void Interprete::saveDisplaced(std::string instruction) {
    char* type = new char[1];
    char* memrefdst = new char[15];
    char* memrefsrc = new char[15];
    char* integer = new char[15];
    std::strcpy(type, instruction.substr(4, 1).c_str());
    std::strcpy(memrefdst, instruction.substr(5, 15).c_str());
    std::strcpy(integer, instruction.substr(20, 15).c_str());
    std::strcpy(memrefsrc, instruction.substr(35, 15).c_str());

    std::cout << "2.2.8 saveDisplaced: type memrefdst memrefsrc integer -> " 
        << type << " "
        << memrefdst << " " 
        << memrefsrc << " "  
        << integer << std::endl;

    int offset = memoria->readInt(LITNUM, integer);
    if(type == "1") {
        int value = memoria->readInt(DATANUM, memrefsrc);
        memoria->writeInt(DATANUM, (memrefdst + offset), value);
    } else {
        char value = memoria->readChar(DATASTR, strtol(memrefsrc, nullptr, 2));
        memoria->writeChar(DATASTR, strtol(memrefdst, nullptr, 2) + offset, value);
    }
}

void Interprete::readStr(std::string instruction) {
    char* memrefdst    = new char[15];
    char* memrefsize   = new char[15]; 
    std::strcpy(memrefdst, instruction.substr(4, 15).c_str());
    std::strcpy(memrefsize, instruction.substr(19, 15).c_str());

    std::cout << "2.2.11 ReadStr: memrefdst memrefsize -> " << memrefdst << " " << memrefsize << std::endl;

    char charActual;
    int limite = memoria->readInt(LITNUM, memrefsize);
    int memDstVal = strtol(memrefdst, nullptr, 2);
    int i = 0;
    int j = 0;

    std::string cadena;
    std::getline(std::cin, cadena);

    for(char& c : cadena) {
        if (c && i < limite) {
            int newMemDst = memDstVal + (((j * 4) + (4 - (i % 4))) - 1);

            memoria->writeChar(DATASTR, newMemDst, c);

            i++;
            if (i % 4 == 0) j++;
        }
    }
    memoria->writeChar(DATASTR, memDstVal + i + 1, '\0');
}

void Interprete::conditionalJump(std::string instruction) {
    char* operation = new char[3];
    char* flag = new char[1];
    char* memrefop1 = new char[15];
    char* memrefop2 = new char[15];
    char* integer = new char[15];
    std::strcpy(operation, instruction.substr(4, 3).c_str());
    std::strcpy(flag, instruction.substr(7, 1).c_str());
    std::strcpy(memrefop1, instruction.substr(8, 15).c_str());
    std::strcpy(memrefop2, instruction.substr(23, 15).c_str());
    std::strcpy(integer, instruction.substr(38, 15).c_str());

    std::cout << "2.2.14 ConditionalJump: op flag memrefop1 memrefop2 memrefinteger -> " 
        << operation << " " 
        << flag << " " 
        << memrefop1 << " " 
        << memrefop2 << " " 
        << integer << std::endl;               

    if (flag == "1") {
        int op1 = memoria->readInt(DATANUM, memrefop1);
        int op2 = memoria->readInt(DATANUM, memrefop2);
        int value = memoria->readInt(LITNUM, integer);
        int result;
        if (operation == "000") {
            program_counter = op1 >= op2 ? value : program_counter;
        } else if(operation == "001") {
            program_counter = op1 > op2 ? value : program_counter;
        } else if (operation == "010") {
            program_counter = op1 <= op2 ? value : program_counter;
        } else if (operation == "011") {
            program_counter = op1 < op2 ? value : program_counter;
        } else if (operation == "100") {
            program_counter = op1 == op2 ? value : program_counter;
        } else if (operation == "101") {
            program_counter = op1 != op2 ? value : program_counter;
        }
    } else {    
        char op1 = memoria->readChar(DATASTR, strtol(memrefop1, nullptr, 2));
        char op2 = memoria->readChar(DATASTR, strtol(memrefop2, nullptr, 2));
        int value = memoria->readInt(LITNUM, integer);
        char result;
        if (operation == "000") {
            program_counter = op1 >= op2 ? value : program_counter;
        } else if(operation == "001") {
            program_counter = op1 > op2 ? value : program_counter;
        } else if (operation == "010") {
            program_counter = op1 <= op2 ? value : program_counter;
        } else if (operation == "011") {
            program_counter = op1 < op2 ? value : program_counter;
        } else if (operation == "100") {
            program_counter = op1 == op2 ? value : program_counter;
        } else if (operation == "101") {
            program_counter = op1 != op2 ? value : program_counter;
        }
    }
}