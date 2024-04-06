#define MAP "../t_data/MM_TEST1.bmp"

#include <iostream>
#include "labirynth_operations.h"

Tile exp_algorythm (Tile [][LAB_SIZE]);
void floodfill (Tile [][LAB_SIZE]);

int main (int argc, char *argv[]){

	std::cout << "Hello, world! \n";
	

	std::cout << "Labirynth import testing \n";

	Tile labirynth[LAB_SIZE][LAB_SIZE]; // string operations for testing streamlining (for map1, map2... -> path1.bmp, path2.bmp...)
	import_labirynth(MAP, labirynth);
 	
 	Tile interpreted = exp_algorythm(labirynth);
 	floodfill(interpreted);

	print_path("./path.bmp", interpreted);

	return 0;
}

Tile exp_algorythm (Tile external[][LAB_SIZE]){
	Tile internal[LAB_SIZE][LAB_SIZE];

	return internal;
}
