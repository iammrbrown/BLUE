#define MAP "../t_data/MM_TEST1.bmp"

#include <iostream>
#include "labirynth_operations.h"
#include "mouse_operations.h"

void flood_algorythm (Tile [][LAB_SIZE]);

int main (int argc, char *argv[]){

	std::cout << "Hello, world!" << std::endl;
	

	std::cout << "Labirynth flood testing" << std::endl;

	Tile labirynth[LAB_SIZE][LAB_SIZE]; // string operations for testing streamlining (for map1, map2... -> path1.bmp, path2.bmp...)
	import_labirynth(MAP, labirynth);

 	flood_std_labirynth(labirynth);

 	for(int y = LAB_SIZE-1; y >= 0; y--){
 		for(int x = 0; x < LAB_SIZE; x++){
 			if(labirynth[x][y].flood < 10){std::cout << labirynth[x][y].flood << "  ";}
 			else{std::cout << labirynth[x][y].flood << " ";}
 		}
 		std::cout << std::endl;
 	}

 	/*Tile interpreted[LAB_SIZE][LAB_SIZE];
 	flood_algorythm(labirynth, interpreted);

	print_path("./path.bmp", interpreted);*/

	return 0;
}

void flood_algorythm (Tile external[][LAB_SIZE], Tile internal[][LAB_SIZE]){

}
