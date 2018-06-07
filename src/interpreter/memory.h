#include <map>
#include <vector>

struct Segment {
    int base;
    int limit;
};

struct MMU {
    std::map<int, Segment> table;
};

class Memory {
    private:
        char* name;
        int* pMem;
        MMU* mmu;

        int* init(char* name, int size);
        void create_mmu(std::vector<Segment> segments);
    
    public:
        Memory(char* newName, int size, std::vector<Segment> segments);
};