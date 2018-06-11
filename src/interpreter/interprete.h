#include <string>
#include <vector>

#include "memory.h"

class Interprete {
    private:
        std::vector<std::string> instructions;
        Memory* memoria;
        int program_counter;
        const int MEMG = std::stoi("000", nullptr, 2);
        const int LITNUM = std::stoi("001", nullptr, 2);
        const int LITSTR = std::stoi("010", nullptr, 2);
        const int DATANUM = std::stoi("011", nullptr, 2);
        const int DATASTR = std::stoi("100", nullptr, 2);
        const int WORKLOAD = std::stoi("101", nullptr, 2);
        void init(char* filename);

    public: 
        Interprete(char* filename, char* memName);
        void run();
        void haltInst();
        void breakInst();
        void memrefToPc(std::string instruction);
        void readInt(std::string instruction);
        void writeInt(std::string instruction);
        void writeStr(std::string instruction);
        void goTo(std::string instruction);
        void moveNum(std::string instruction);
        void moveStr(std::string instruction);                                        
        void saveNumPlusPC(std::string instruction);            
        void moveValueInMemory(std::string instruction);
        void arithmeticOp(std::string instruction);        
        void moveValueInMemoryInteger(std::string instruction);        
        void saveDisplaced(std::string instruction);
        void readStr(std::string instruction);
        void conditionalJump(std::string instruction);
};