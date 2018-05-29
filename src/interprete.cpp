#include <bitset>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

using namespace std;

// 8 bits instructions
void haltInst() {
    cout << "2.2.15 HALT" << endl; }

void breakInst() {
    cout << "2.2.4 BREAK " << endl;
}

// 32 bits instructions
void memrefToPc(string instruction) {
    string memref = instruction.substr(4, 15);
    cout << "2.2.4 <memref> " << memref << endl;
}

void readInt(string instruction) {
    string memref = instruction.substr(4, 15);
    cout << "2.2.9 <memref> " << memref << endl;
}

void writeInt(string instruction) {
    string memref = instruction.substr(4, 15);
    cout << "2.2.10 <memref> " << memref << endl;
}

void writeStr(string instruction) {
    string memref = instruction.substr(4, 15);
    cout << "2.2.12 <memref> " << memref << endl;
}

void goTo(string instruction) {
    string address = instruction.substr(4, 15);
    cout << "2.2.13 <address> " << address << endl;
}

// 64 bits instructions

void moveNum(string instruction) {
    string memref = instruction.substr(4, 15);
    string integer = instruction.substr(19, 15);
    cout << "2.2.1 <memref> " << memref 
            << " <integer> " << integer << endl;
}

void moveStr(string instruction) {
    string memref = instruction.substr(4, 15);
    string strRef = instruction.substr(19, 15);
    cout << "2.2.2 <memref> " << memref 
            << " <string> " << strRef << endl;
}

void saveNumPlusPC(string instruction) {
    string memref = instruction.substr(4, 15);
    string integer = instruction.substr(19, 15);
    cout << "2.2.3 <memref> " << memref 
            << " <string> " << integer << endl;
}

void moveValueInMemory(string instruction) {
    string type         = instruction.substr(4,1);
    string memrefdst    = instruction.substr(5, 15);
    string memrefsrc    = instruction.substr(20, 15);
    cout << "2.2.4 <type> " << type 
            << " <memrefdst> " << memrefdst 
            << " <memrefsrc> " << memrefsrc << endl;
}

void arithmeticOp(string instruction) {
    string operation    = instruction.substr(4, 3);
    string flag         = instruction.substr(7, 1);
    string memrefdst    = instruction.substr(8, 15);
    string memrefop1    = instruction.substr(24, 15);
    string memrefop2    = instruction.substr(39, 15);
    cout << "2.2.6 <op> " << operation 
            << " <memrefdst> " << memrefdst 
            << " <memrefop1> " << memrefop1
            << " <memrefop2> " << memrefop2 << endl;
}

void moveValueInMemoryInteger(string instruction) {
    string type         = instruction.substr(4,1);
    string memrefdst    = instruction.substr(5, 15);
    string memrefsrc    = instruction.substr(20, 15);
    string integer      = instruction.substr(35, 15);
    cout << "2.2.7 <type> " << type 
            << " <memrefdst> " << memrefdst 
            << " <memrefsrc> " << memrefsrc
            << " <integer> " << integer << endl;
}

void saveDisplaced(string instruction) {
    string type         = instruction.substr(4,1);
    string memrefdst    = instruction.substr(5, 15);
    string integer      = instruction.substr(20, 15);
    string memrefsrc    = instruction.substr(35, 15);
    cout << "2.2.8 <type> " << type 
            << " <memrefdst> " << memrefdst 
            << " <integer> " << integer
            << " <memrefsrc> " << memrefsrc << endl;
}

void readStr(string instruction) {
    string memrefdst    = instruction.substr(4, 15);
    string memrefsize   = instruction.substr(19, 15);
    cout << "2.2.11 <memrefdst> " << memrefdst 
            << " <memrefsize> " << memrefsize << endl;
}

void conditionalJump(string instruction) {
    string operation    = instruction.substr(4, 3);
    string memref1    = instruction.substr(7, 15);
    string memref2    = instruction.substr(22, 15);
    string integer      = instruction.substr(37, 15);
    cout << "2.2.14 <op> " << operation 
            << " <memref1> " << memref1 
            << " <memref2> " << memref2
            << " <integer> " << integer << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
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

    for(vector<string>::size_type i = 0; i <= instructions.size(); ++i) {
        string instruction = instructions[i];
        
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

    return EXIT_SUCCESS;
}