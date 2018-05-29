#include <bitset>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

#include "decoder.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <file>" << endl;
        return EXIT_FAILURE;
    }

    char* filename = argv[1];
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

    return EXIT_SUCCESS;
}