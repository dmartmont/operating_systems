#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(void) {
			// 0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
			// 0x60, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
			// 0x70, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			// 0x80, 0xA8, 0x4E, 0x62, 
			// 0x90, 0xFF, 0xFF, 0xFF, 
			// 0xB0, 0xFF, 0xFF, 0xFF, 
			// 0xC0, 0xFF, 0xFF, 0xFF,
			// 0xE0
			// 0xF0

  	char prog[] = {
		  		0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
				0x60, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
				0x70, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0x80, 0xA8, 0x4E, 0x62, 
				0x90, 0xFF, 0xFF, 0xFF, 
				0xB0, 0xFF, 0xFF, 0xFF, 
				0xC0, 0xFF, 0xFF, 0xFF,
				0xE0,
				0xF0
			};

	ofstream fileprog("instructions.bew", ios_base::binary);

	fileprog.write(prog, sizeof(prog));
	fileprog.close();

	return EXIT_SUCCESS;
}