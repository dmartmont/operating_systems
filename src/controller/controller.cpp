#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>

#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <unistd.h>
#include <sys/types.h>

using namespace std;

int* create_memory(char* name) {
    int shm = shm_open(name, O_CREAT | O_RDWR | O_EXCL, 0600);

    if (shm == -1) {
        cerr << "Shared memory already created" << endl;
        return (int*)-1;
    }

    off_t size_mem = 1000;

    if (ftruncate(shm, size_mem) == -1) {
        cerr << "Problems with memory size" << endl;
        return (int*)-1;
    }

    char *pMem = static_cast<char*>(mmap(NULL, size_mem, PROT_READ | PROT_WRITE,
                                        MAP_SHARED, shm, 0));

    if ((void *) pMem == (void *) -1) {
        cerr << "Problems with memory map" << endl;
        return (int*)-1;
    }

    return (int*)pMem;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <memshared> <memfile>" << endl;
        return EXIT_FAILURE;
    }

    int* pMem = create_memory(argv[1]);

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

    int* data = (int *)inputData;

    for (int i = 0; i < 6; ++i) {
        int* segment = data + i;

        int base  = (*segment >> 16) << 2;
        int limit = ((*segment << 16) >> 16);

        if(i == 2 || i == 4) {
            limit = limit + (4 - limit % 4);
        } else {
            limit <<= 2;
        }

        cout << "Base: " << hex << base << " Limit: " << hex << limit << endl;
    }
    
    return EXIT_SUCCESS;
}