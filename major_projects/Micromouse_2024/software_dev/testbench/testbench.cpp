#define MAP "../t_data/MM_TEST2.bmp"

#include <iostream>
#include "labirynth_operations.h"
#include "mouse_operations.h"

void flood_algorythm (Tile [][LAB_SIZE], Tile [][LAB_SIZE]);

int main (int argc, char *argv[]){

	std::cout << "Hello, world!" << std::endl;
	

	std::cout << "Labirynth flood testing" << std::endl;

	Tile labirynth[LAB_SIZE][LAB_SIZE]; // string operations for testing streamlining (for map1, map2... -> path1.bmp, path2.bmp...)
	import_labirynth(MAP, labirynth);

 	Tile interpreted[LAB_SIZE][LAB_SIZE];
 	flood_algorythm(labirynth, interpreted);

	print_path("./path.bmp", labirynth);

	return 0;
}

void flood_algorythm (Tile external[][LAB_SIZE], Tile internal[][LAB_SIZE]){
	set_std_internal(internal);
	Mouse mouse;
	int step = 0;

	while(true){
		discover_cell(mouse, external, internal);
		update_walls(internal);
		external[mouse.x][mouse.y].step = step; step ++;
		flood_std_labirynth(internal);
		print_flood_map(internal);

		std::cout << "\n" << mouse.x << " " << mouse.y << " | ";
		if(internal[mouse.x][mouse.y].END){
			break;
		}
			 if(internal[mouse.x][mouse.y].N == false && (internal[mouse.x][mouse.y].flood - internal[mouse.x][mouse.y+1].flood) == 1){mouse = face_direction(mouse,'N'); mouse = go_ahead(mouse);}
		else if(internal[mouse.x][mouse.y].E == false && (internal[mouse.x][mouse.y].flood - internal[mouse.x+1][mouse.y].flood) == 1){mouse = face_direction(mouse,'E'); mouse = go_ahead(mouse);}
		else if(internal[mouse.x][mouse.y].S == false && (internal[mouse.x][mouse.y].flood - internal[mouse.x][mouse.y-1].flood) == 1){mouse = face_direction(mouse,'S'); mouse = go_ahead(mouse);}
		else if(internal[mouse.x][mouse.y].W == false && (internal[mouse.x][mouse.y].flood - internal[mouse.x-1][mouse.y].flood) == 1){mouse = face_direction(mouse,'W'); mouse = go_ahead(mouse);}
		else{
			std::cout << "Mamy problem: MOUSE IS STUCK?\n"; 
			print_labirynth("./crash.bmp", internal); 
			std::cout << internal[mouse.x][mouse.y].N << internal[mouse.x][mouse.y].E << internal[mouse.x][mouse.y].S << internal[mouse.x][mouse.y].W << "\n";
			std::cout << internal[mouse.x][mouse.y].flood << "\n";
			break;}
	}
}
