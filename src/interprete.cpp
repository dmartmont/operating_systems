#include <bitset>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

using namespace std;

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
            cout << i << " - 8 bits " << value.to_string() << endl;
            ++i;
        } 
        // 32 bits intructions
        else if(opcode == (char)0x03 || opcode == (char)0x08 || opcode == (char)0x09 || 
                opcode == (char)0x0B || opcode == (char)0x0C) {

            string instruction ="";
            
            for (int j = i; j < i + 4; ++j) {
                instruction.append(bitset<8>(inputData[j]).to_string());
            }

            instructions.push_back(instruction);
            cout << i << " - 32 bits " << instruction << endl;
            i += 4;
        } 
        else if(opcode == (char)0x00 || opcode == (char)0x01 || opcode == (char)0x02 || 
                opcode == (char)0x04 || opcode == (char)0x05 || opcode == (char)0x06 ||
                opcode == (char)0x07 || opcode == (char)0x0A || opcode == (char)0x0D) {

            string instruction ="";
            
            for (int j = i; j < i + 8; ++j) {
                instruction.append(bitset<8>(inputData[j]).to_string());
            }

            instructions.push_back(instruction);
            cout << i << " - 64 bits " << instruction << endl;
            i += 8;
        }
    }

    return EXIT_SUCCESS;
}