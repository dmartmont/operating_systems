#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

#include "interprete.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <nommemcom> <bewfile>" << endl;
        return EXIT_FAILURE;
    }

    char* memName = argv[1];
    char* filename = argv[2];
    
    Interprete* interprete = new Interprete(filename, memName);

    interprete->run();

    delete(interprete);

    return EXIT_SUCCESS;
}