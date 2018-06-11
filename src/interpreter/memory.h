#include <map>
#include <vector>

struct Segment {
    int base;
    int limit;
    bool isInside(int position) {
        return position < limit;
    }
};

struct MMU {
    std::map<int, Segment> table;
    int getPositionReal(int segmentNumber, int movement) {
        Segment segment = table[segmentNumber];
        if (segment.isInside(movement)) {
            return segment.base + movement;
        } else {
            return -1;
        }
    }
};

class Memory {
    private:
        char* name;
        char* pMem;
        struct MMU mmu;

        char* init(char* name, int size);
        void create_mmu(std::vector<Segment> segments);
    
    public:
        Memory(char* newName, int size, std::vector<Segment> segments);
        int readInt(int segment, char* offset);
        void writeInt(int segment, char* offset, int value);
        char readChar(int segment, int offset);
        void writeChar(int segment, int offset, char value);
};