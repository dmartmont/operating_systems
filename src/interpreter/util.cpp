#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <ostream>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "util.h"

int* open_shared_memory(char* name, int size) {
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