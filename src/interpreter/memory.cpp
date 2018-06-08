#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <vector>

#include "memory.h"

Memory::Memory(char* newName, int size, std::vector<Segment> segments) {
    name = newName;
    pMem = init(name, size);
    create_mmu(segments);

    if ((void*) pMem != (void*) -1) {
        std::cout << "Memory succesfully abierta" << std::endl;
    } else { 
        std::cout << "Fallé como un idiota" << std::endl;
    }
}        


int* Memory::init(char* name, int size) {
    int shm = shm_open(name, O_RDWR , 0600);

    if (shm == -1) {
        std::cerr << "Failed to open shared memory " << name << std::endl;
        return (int*)-1;
    }

    off_t size_mem = (off_t) size;

    char* pMem = static_cast<char*>(mmap(NULL, size_mem, PROT_READ | PROT_WRITE,
                                        MAP_SHARED, shm, 0));

    if ((void *) pMem == (void *) -1) { 
        std::cerr << "Problems with memory map ("
            << errno << ") "
            << strerror(errno)
            << std::endl;
        return (int*)-1;
    }
    return (int*)pMem;
}

void Memory::create_mmu(std::vector<Segment> segments) {
    for (int i = 0; i < 6; ++i) {
        mmu.table.insert(std::pair<int, struct Segment>(i, segments[i]));
    }
}

void Memory::writeChar(int segment, char* offset, char* value) {
    int movement = strtol(offset, nullptr, 16);
    int real = mmu.getPositionReal(segment, movement);

}

char* Memory::readChar(int segment, char* offset) {
    int movement = strtol(offset, nullptr, 16);
    int real = mmu.getPositionReal(segment, movement);
    return (char*)-1;
}

void Memory::writeInt(int segment, char* offset, int value) {
    int movement = strtol(offset, nullptr, 16);
    int real = mmu.getPositionReal(segment, movement);

    if (real == -1) {
        std::cerr << "Wrong memory position writing int" << std::endl;
    } else {
        *(pMem + real) = value;
    }
}

int Memory::readInt(int segment, char* offset) {
    int movement = strtol(offset, nullptr, 16);
    int real = mmu.getPositionReal(segment, movement);

    if (real == -1) {
        std::cerr << "Wrong memory position reading int" << std::endl;
        return -1;
    }

    return *(pMem + real);
}