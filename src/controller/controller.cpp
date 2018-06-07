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

int* create_memory(char* name, int size) {
    int shm = shm_open(name, O_CREAT | O_RDWR | O_EXCL, 0600);

    if (shm == -1) {
        cerr << "Shared memory already created" << endl;
        return (int*)-1;
    }

    off_t size_mem = size;

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

    int* data = (int*)inputData;

    int* final_segment = data + 5;
    int final_base = (*final_segment >> 16) << 2;
    int final_limit = (*final_segment << 16) >> 16;
    int final_size = final_base + final_limit;

    int* pMem = create_memory(argv[1], final_size);
    int* confMem = create_memory((char*)"config", 28);

    *confMem = final_size;
    confMem = confMem + 1;

    for (int i = 0; i < 6; ++i) {
        int* segment = data + i;

        *(confMem + i) = *segment;
    }

    for (int i = 0; i < size; ++i) {
        *(pMem + i) = *(data + i);
    }
    
    return EXIT_SUCCESS;
}