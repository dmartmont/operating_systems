#include <string.h>
#include <iostream>

using namespace std;

class Decoder {

    public: 
        // 8 bit instructions
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
};