#define MAP "../t_data/MM_TEST1.bmp"

#include <iostream>
#include "labirynth_operations.h"

int main (int argc, char *argv[]){

	std::cout << "Hello, world! \n";
	

	std::cout << "Labirynth import testing \n";

	Tile labirynth[LAB_SIZE][LAB_SIZE];
	import_labirynth(MAP, labirynth);
 	
	A_left_handed(labirynth);

	print_path("./path.bmp", labirynth);

	return 0;
}