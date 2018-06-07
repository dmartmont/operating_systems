#include <bitset>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "decoder.cpp"
#include "memory.h"

using namespace std;

int* open_shared_memory(char* name, int size) {
    int shm = shm_open(name, O_RDWR , 0600);

    if (shm == -1) {
        cerr << "Failed to open shared memory " << name << endl;
        return (int*)-1;
    }

    off_t size_mem = (off_t) size;

    char* pMem = static_cast<char*>(mmap(NULL, size_mem, PROT_READ | PROT_WRITE,
                                        MAP_SHARED, shm, 0));

    if ((void *) pMem == (void *) -1) { 
        cerr << "Problems with memory map ("
            << errno << ") "
            << strerror(errno)
            << endl;
        return (int*)-1;
    }
    return (int*)pMem;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <nommemcom> <bewfile>" << endl;
        return EXIT_FAILURE;
    }

    char* memName = argv[1];
    char* filename = argv[2];
    ifstream is;
    size_t size;

    is.open(filename, ios::binary | ios::in | ios::ate);

    if (!is.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return EXIT_FAILURE;
    }

    size = is.tellg();
    char* inputData = new char[size];

    is.seekg(0, ios::beg);
    is.read(inputData, size);
    inputData[size] = '\0';

    size_t i = 0;
    vector<string> instructions;

    while (i < size) {
        char opcode = (inputData[i]);

        opcode = (opcode >> 4) & 0x0F;

        // 8 bits instructions
        if(opcode == (char)0x0F || opcode == (char)0x0E) {
            bitset<8> value = bitset<8>(inputData[i]);
            instructions.push_back(value.to_string());
            ++i;
        } 
        // 32 bits intructions
        else if(opcode == (char)0x03 || opcode == (char)0x08 || 
                opcode == (char)0x09 || opcode == (char)0x0B || 
                opcode == (char)0x0C) {

            string instruction ="";
            
            for (int j = i; j < i + 4; ++j) {
                instruction.append(bitset<8>(inputData[j]).to_string());
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

            string instruction ="";
            
            for (int j = i; j < i + 8; ++j) {
                instruction.append(bitset<8>(inputData[j]).to_string());
            }

            instructions.push_back(instruction);
            i += 8;
        }
    }

    Decoder deco;

    for(vector<string>::size_type i = 0; i <= instructions.size(); ++i) {
        string instruction = instructions[i];
        
        // 8 bits instructions
        if(instruction.substr(0, 4) == "1110") {
            deco.haltInst();
        }
        else if(instruction.substr(0, 4) == "1111") {
            deco.breakInst();
        }
        // 32 bits instructions
        else if(instruction.substr(0, 4) == "0011") {
            deco.memrefToPc(instruction);
        }
        else if(instruction.substr(0, 4) == "1000") {
            deco.readInt(instruction);
        }
        else if(instruction.substr(0, 4) == "1001") {
            deco.writeInt(instruction);
        }
        else if(instruction.substr(0, 4) == "1011") {
            deco.writeStr(instruction);
        }
        else if(instruction.substr(0, 4) == "1100") {
            deco.goTo(instruction);
        }
        // 64 bits instructions
        else if(instruction.substr(0, 4) == "0000") {
            deco.moveNum(instruction);
        }
        else if(instruction.substr(0, 4) == "0001") {
            deco.moveStr(instruction);
        }
        else if(instruction.substr(0, 4) == "0010") {
            deco.saveNumPlusPC(instruction);
        }
        else if(instruction.substr(0, 4) == "0100") {
            deco.moveValueInMemory(instruction);
        }
        else if(instruction.substr(0, 4) == "0101") {
            deco.arithmeticOp(instruction);
        }
        else if(instruction.substr(0, 4) == "0110") {
            deco.moveValueInMemoryInteger(instruction);
        }
        else if(instruction.substr(0, 4) == "0111") {
            deco.saveDisplaced(instruction);
        }
        else if(instruction.substr(0, 4) == "1010") {
            deco.readStr(instruction);            
        }
        else if(instruction.substr(0, 4) == "1101") {
            deco.conditionalJump(instruction);
        }
    }

    int* configMem = open_shared_memory((char*)"config", 28);

    // Size of shared memory
    int memSize = *configMem;
    configMem += 1;

    vector<Segment> segments;

    for (int i = 0; i < 6; ++i) {
        int segment = *configMem;

        struct Segment segmentObj;

        segmentObj.base = (segment >> 16);
        int limit = ((segment << 16) >> 16);

        if(i == 2 || i == 4) {
            limit = limit + (4 - limit % 4);
        } else {
            limit <<= 2;
        }

        segmentObj.limit = limit;

        segments.push_back(segmentObj); 
    }

    Memory* memoria = new Memory(memName, memSize, segments);

    return EXIT_SUCCESS;
}