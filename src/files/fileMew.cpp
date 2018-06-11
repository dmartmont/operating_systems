#include <fstream>
#include <cstdlib>

using namespace std;

int main(void) {
	char prog[] =  {
	// Base Limite 	-> 	Limite Base
	// 00 00 00 0F 	-> 	0F 00  00 00 << 2
	// 					3C 00
	
	// --------
	// 3  2   1  0 		0  1  2  3
	// Limite Base 	-> 	Base  Limite
	// 0F 00  00 00	-> 	00 00 00 0F << 2
	// 						  00 3C

	// .mew en Little Endian
	//  Limite 		Base
		0x0F, 0x00, 0x00, 0x00,             //Tamaño MemG
		0x03, 0x00, 0x0F, 0x00,             //Tamaño lirNum
		0x0B, 0x00, 0x12, 0x00,             //Tamaño litString
		0x04, 0x00, 0x15, 0x00,             //Tamaño dataNum
		0x7D, 0x00, 0x19, 0x00,             //Tamaño dataString
		0x20, 0x00, 0x39, 0x00,             //Tamaño Workload
	// 	Fin 		Inicio
		0x0B, 0x00, 0x0A, 0x00,             //Tamaño Inicio-Fin Lect/E       1      
		0x0C, 0x00, 0x0B, 0x00,             //Tamaño Inicio-Fin L/Escr       1
		0x0E, 0x00, 0x0C, 0x00,             //Tamaño Inicio-Fin Bloqueado    2
		0x0F, 0x00, 0x0E, 0x00,             //Tamaño Inicio-Fin Sin Politica 1
	//  Fin 		Inicio
		0x80, 0x00, 0x80, 0x00,             //Segmento 1 Lect/E 
		0x80, 0x01, 0x80, 0x01,             //Segmento 1 Lect/Escr
		0x80, 0x02, 0x80, 0x02,             //Segmento 1 Bloqueado
		0x00, 0x00, 0x00, 0x7D,             //Segmento 2 Bloqueado
		0x80, 0x03, 0x80, 0x03,             //Segmento 1 No Bloqueado 
		0xFF, 0xFF, 0xFF, 0xFF,             //Inicio litNum
		0xFF, 0xFF, 0xFF, 0x7F,             //       litNum
		0x00, 0x00, 0x00, 0x00,             //       litNum
		0x61, 0x6C, 0x6F, 0x48,             //Inicio litStr
		0x6E, 0x75, 0x6D, 0x20,             //       litStr
		0x00, 0x00, 0x6F, 0x64              //       litStr
	};

	ofstream fileprog("memoria.mew", ios_base::binary);

	fileprog.write(prog, sizeof(prog));
	fileprog.close();

	return EXIT_SUCCESS;
}