#include <iostream>
#include <fstream>
#include <string>

#ifndef BMAP_SIZE
#define BMAP_SIZE 256
#endif

#ifndef LAB_SIZE
#define LAB_SIZE 16
#endif

#ifndef BMAP_BUFF
#define BMAP_BUFF 20
#endif

#ifndef HUE_STEP
#define HUE_STEP 10
#endif

#ifndef EXP_STEPS
#define EXP_STEPS 3000
#endif

#ifndef MAP
#define MAP "../t_data/MM_base.bmp"
#endif

/* 	In this file there is a set of functions and solutions related to operations on
	BMP files representing test labirynths for the Micromouse projects.				*/

#ifndef TILE
#define TILE

typedef struct {
	bool N;
	bool E;
	bool S;
	bool W;
	bool START = false;
	bool END = false;
	int step = 0;
	int flood = 0;
	bool IGNORED = false;
} Tile;

#endif

#ifndef LABIRYTNH_OP_H
#define LABIRYTNH_OP_H

typedef struct{								
	uint32_t 	size = 0;					// rozmiar pliku w bajtach				
	uint32_t	reserved = 0; 				// można swobodnie wyzerować, nieważne w tym zastosowaniu
	uint32_t	offset = 52;				// offset danych pikseli od początku pliku
	uint32_t	dib_header_size = 40;		// długość headera informacji o bitmapie
	int32_t 	width_px = LAB_SIZE*4+1;	// szerokość bitmapy (w pikselach)
	int32_t 	height_px = LAB_SIZE*4+1;	// wysokość  bitmapy (w pikselach)
	uint16_t	num_planes = 1;				// coś skomplikowanego, związanego z ilością płaszczyzn kolorów. jedna dla wygody.
	uint16_t 	bits_per_pixel = 24;		// ilość bitów przeznaczanych na każdy piksel.
	uint32_t	compression = 0;			// 0 dla naszych zastosowań
	uint32_t	image_size_bytes = 0;	 	// przy compression = 0 ten parametr i tak jest ignorowany
	int32_t 	x_resolution_ppm = 0; 		// znaczenie w druku - rozdzielczość w pikselach na metr X
	int32_t 	y_resolution_ppm = 0; 		// znaczenie w druku - rozdzielczość w pikselach na metr X
	uint32_t	num_colors = 0;				// po prawdzie - nie mam pojęcia
	uint32_t 	important_colors = 0; 		// tu tak samo, ale działa
}BMPHeader;

typedef struct{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}pixel;

pixel hue_rotation (unsigned int hue){
	hue = hue % 1530;
	pixel rgb;

	if(hue < 255){
		hue = hue % 255;
		rgb.R = 255;
		rgb.G = hue;
		rgb.B = 0;
		std::cout << "1: ";
	}else if(hue < 510){
		hue = hue % 255;
		rgb.R = 255-hue;
		rgb.G = 255;
		rgb.B = 0;
		std::cout << "2: ";
	}else if(hue < 765){
		hue = hue % 255;
		rgb.R = 0;
		rgb.G = 255;
		rgb.B = hue;
		std::cout << "3: ";
	}else if(hue < 1020){
		hue = hue % 255;
		rgb.G = 255-hue;
		rgb.B = 255;
		rgb.R = 0;
		std::cout << "4: ";
	}else if(hue < 1275){
		hue = hue % 255;
		rgb.R = hue;
		rgb.G = 0;
		rgb.B = 255;
		std::cout << "5: ";
	}else{
		hue = hue % 255;
		rgb.B = 255-hue;
		rgb.R = 255;
		rgb.G = 0;
		std::cout << "6: ";
	}


	std::cout << (int) rgb.R << " " << (int) rgb.G << " " << (int) rgb.B << "\n";
	return rgb;
}

/* SAMPLE ALGORYTHMS */

void A_right_handed (Tile labirynth[][LAB_SIZE]){
	char facing = 'N';
	char right = 'E';

	int x = 0;
	int y = 0;
	int step = 0;

	while(labirynth[x][y].END != true && step <= EXP_STEPS){
		labirynth[x][y].step = step;

		std::cout << "At: [ " << x << ", " << y << "]; Facing: " << facing << "; N: " << labirynth[x][y].N << ", E: " << labirynth[x][y].E << ", S: " << labirynth[x][y].S << ", W: " << labirynth[x][y].W << std::endl;
		switch(facing){
		case 'N':
			right = 'E';
			if(labirynth[x][y].E == true){
				if(labirynth[x][y].N == false){y++;}
				else{facing = 'W';}
			}else{
				facing = right;
				x++;
			}
			break;
		case 'E':
			right = 'S';
			if(labirynth[x][y].S == true){
				if(labirynth[x][y].E == false){x++;}
				else{facing = 'N';}
			}else{
				facing = right;
				y--;
			}
			break;
		case 'S':
			right = 'W';
			if(labirynth[x][y].W == true){
				if(labirynth[x][y].S == false){y--;}
				else{facing = 'E';}
			}else{
				facing = right;
				x--;
			}
			break;
		case 'W':
			right = 'N';
			if(labirynth[x][y].N == true){
				if(labirynth[x][y].W == false){x--;}
				else{facing = 'S';}
			}else{
				facing = right;
				y++;
			}
			break;	
		default:
			std::cout << "Mamy problem: A_RH - BAD FACING" << std::endl;
		}

		std::cout << "Step: " << step << " Position: " << x << ", " << y << " Facing: " << facing << "\n";
		if(x < 0 || x > 15 || y < 0 || y > 15){break;}
		step++;
	}

	if(labirynth[x][y].END == true){
		std::cout << "Finished in " << step << " steps.\n";
	}else{
		std::cout << "DNF in " << EXP_STEPS << " steps.\n";
	}
}

void A_left_handed (Tile labirynth[][LAB_SIZE]){
	char facing = 'N';
	char left = 'E';

	int x = 0;
	int y = 0;
	int step = 0;

	while(labirynth[x][y].END != true && step <= EXP_STEPS){
		labirynth[x][y].step = step;

		std::cout << "At: [ " << x << ", " << y << "]; Facing: " << facing << "; N: " << labirynth[x][y].N << ", E: " << labirynth[x][y].E << ", S: " << labirynth[x][y].S << ", W: " << labirynth[x][y].W << std::endl;
		switch(facing){
		case 'N':
			left = 'W';
			if(labirynth[x][y].W == true){
				if(labirynth[x][y].N == false){y++;}
				else{facing = 'E';}
			}else{
				facing = left;
				x--;
			}
			break;
		case 'E':
			left = 'N';
			if(labirynth[x][y].N == true){
				if(labirynth[x][y].E == false){x++;}
				else{facing = 'S';}
			}else{
				facing = left;
				y++;
			}
			break;
		case 'S':
			left = 'E';
			if(labirynth[x][y].E == true){
				if(labirynth[x][y].S == false){y--;}
				else{facing = 'W';}
			}else{
				facing = left;
				x++;
			}
			break;
		case 'W':
			left = 'S';
			if(labirynth[x][y].S == true){
				if(labirynth[x][y].W == false){x--;}
				else{facing = 'N';}
			}else{
				facing = left;
				y--;
			}
			break;	
		default:
			std::cout << "Mamy problem: A_RH - BAD FACING" << std::endl;
		}

		std::cout << "Step: " << step << " Position: " << x << ", " << y << " Facing: " << facing << "\n";
		if(x < 0 || x > 15 || y < 0 || y > 15){break;}
		step++;
	}

	if(labirynth[x][y].END == true){
		std::cout << "Finished in " << step << " steps.\n";
	}else{
		std::cout << "DNF in " << EXP_STEPS << " steps.\n";
	}
}

/* LABIRYNTH OPERATIONS */

void flood_std_labirynth (Tile labirynth[][LAB_SIZE]){
	labirynth[7][7].flood = 1;
	labirynth[8][7].flood = 1;
	labirynth[7][8].flood = 1;
	labirynth[8][8].flood = 1;

	bool filled = false;
	int active = 1;

	while(!filled){
		filled = true;
		for(int x = 0; x < LAB_SIZE; x++){
			for(int y = 0; y < LAB_SIZE; y++){
				if(labirynth[x][y].flood == active && labirynth[x][y].IGNORED == false){
					if(labirynth[x][y].N == false && labirynth[x][y+1].flood == 0){labirynth[x][y+1].flood = active+1;}
					if(labirynth[x][y].E == false && labirynth[x+1][y].flood == 0){labirynth[x+1][y].flood = active+1;}
					if(labirynth[x][y].S == false && labirynth[x][y-1].flood == 0){labirynth[x][y-1].flood = active+1;}
					if(labirynth[x][y].W == false && labirynth[x-1][y].flood == 0){labirynth[x-1][y].flood = active+1;}
					labirynth[x][y].IGNORED = true;
					filled = false;
				}
			}
		}

		active++;
	}
}

/* BITMAP OPERATIONS */

void interpret_buffer (char in[], char out[]){
	for(int i = 0; i < LAB_SIZE; i++){ //there's a correlation between a labirynth size and the amount of doubled chars (chars holding values of 2 pixels)
		switch(in[i] & 0b11110000){
		case 0x00:
			out[2*i] = 'B';
			break;
		case 0xf0:
			out[2*i] = 'W';
			break;
		case 0xe0:
			out[2*i] = 'C';
			break;
		case 0xa0:
			out[2*i] = 'G';
			break;
		case 0x90:
			out[2*i] = 'R';
			break;
		default:
			out[2*i] = 'X';
		}

		switch(in[i] & 0b00001111){
		case 0x00:
			out[2*i+1] = 'B';
			break;
		case 0x0f:
			out[2*i+1] = 'W';
			break;
		case 0x0e:
			out[2*i+1] = 'C';
			break;
		case 0x0a:
			out[2*i+1] = 'G';
			break;
		case 0x09:
			out[2*i+1] = 'R';
			break;
		default:
			out[2*i+1] = 'X';
		}
	}
	out[32] = 'B'; // <- simplified, I know sides are always black.
}

void import_labirynth (std::string path, Tile labirynth[][LAB_SIZE]){
	std::fstream data_file;
	char buffer[BMAP_BUFF]; // this is an ugly hardcode - this is a line size in 33x33 labirynth file. I'm sure it won't cause a bazillion problems later on xd 
	char row[2*LAB_SIZE+1];

	data_file.open(path, std::ios::binary | std::ios::in);

	if( data_file.good() == true ){

		data_file.seekp(10, std::ios::beg);
		data_file.read(buffer, 1);
		data_file.seekp((int)buffer[0]+20, std::ios::beg); // +20 - preread first (black) line

		for(int x = 0; x < LAB_SIZE; x++){
			labirynth[x][0].S = true;
		}

		for(int y = 0; y < LAB_SIZE; y++){

			// BOTTOM
			if(y != 0){
				for(int x = 0; x < LAB_SIZE; x++){
					labirynth[x][y].S = labirynth[x][y-1].N;
				}
			}

			// MIDDLE
			data_file.read(buffer, 20);
			interpret_buffer(buffer, row);

			for(int x = 0; x < LAB_SIZE; x++){
				switch(row[2*x]){
				case 'B':
					labirynth[x][y].W = true;
					break;
				case 'X':
					std::cout << "Mamy problem: CASE_X: " << x << " " << y << std::endl;
					break;
				default:
					labirynth[x][y].W = false;
				}
				switch(row[2*x+1]){
				case 'C': case 'W':
					break;
				case 'R':
					labirynth[x][y].START = true;
					break;
				case 'G':
					labirynth[x][y].END = true;
					break;
				default:
					std::cout << "Mamy problem: LAB_CENTER_OFF: " << x << " " << y << std::endl;
				}
				if(x!=0){labirynth[x-1][y].E = labirynth[x][y].W;}
			}
			labirynth[LAB_SIZE-1][y].E = true;

			// TOP
			data_file.read(buffer, 20);
			interpret_buffer(buffer, row);

			for(int x = 0; x < LAB_SIZE; x++){
				switch(row[2*x+1]){
				case 'B':
					labirynth[x][y].N = true;
					break;
				case 'X':
					std::cout << "Mamy problem: CASE_X: " << x << " " << y << std::endl;
					break;
				default:
					labirynth[x][y].N = false;
				}
			}

		}

		data_file.close();

	}else std::cout << "The file bloody crashed! \n"; 
}

void draw_wall(pixel buffer[][LAB_SIZE*4+1], char side, int x, int y){
	switch(side){
	case 'N':
		for(int i = 0; i < 5; i++){
			buffer[4*x+i][4*(y+1)].R = 0;
			buffer[4*x+i][4*(y+1)].G = 0;
			buffer[4*x+i][4*(y+1)].B = 0;
		}
		break;
	case 'E':
		for(int i = 0; i < 5; i++){
			buffer[4*(x+1)][4*y+i].R = 0;
			buffer[4*(x+1)][4*y+i].G = 0;
			buffer[4*(x+1)][4*y+i].B = 0;
		}
		break;
	case 'S':
		for(int i = 0; i < 5; i++){
			buffer[4*x+i][4*y].R = 0;
			buffer[4*x+i][4*y].G = 0;
			buffer[4*x+i][4*y].B = 0;
		}
		break;
	case 'W':
		for(int i = 0; i < 5; i++){
			buffer[4*x][4*y+i].R = 0;
			buffer[4*x][4*y+i].G = 0;
			buffer[4*x][4*y+i].B = 0;
		}
		break;
	default:
		std::cout << "Mamy problem: DRAW_WALL - BAD SIDE" << std::endl;
	}
}

void print_labirynth(std::string path, Tile labirynth[][LAB_SIZE]){
	BMPHeader header;
	uint16_t type = 0x4d42;	

	pixel buffer[LAB_SIZE*4+1][LAB_SIZE*4+1];
	pixel path_hue;
	uint8_t filler[3] = {0x69, 0x42, 0xff};

	for(int x = 0; x < LAB_SIZE*4+1; x++){
		for(int y = 0; y < LAB_SIZE*4+1; y++){
			buffer[x][y].R = 255;
			buffer[x][y].G = 255;
			buffer[x][y].B = 255;
		}
	}

	std::ofstream output;

	output.open(path, std::ios::binary | std::ios::out);
	if(output.good() == true){

		for(int x = 0; x < LAB_SIZE; x++){
			for(int y = 0; y < LAB_SIZE; y++){
				if(labirynth[x][y].N == true){draw_wall(buffer, 'N', x, y);}
				if(labirynth[x][y].E == true){draw_wall(buffer, 'E', x, y);}
				if(labirynth[x][y].S == true){draw_wall(buffer, 'S', x, y);}
				if(labirynth[x][y].W == true){draw_wall(buffer, 'W', x, y);}
			}	
		}
		

		output.write((char *) &type, sizeof(uint16_t));
		output.write((char *) &header, sizeof(BMPHeader));

		for(int y = 0; y < LAB_SIZE*4+1; y++){
			for(int x = 0; x < LAB_SIZE*4+1; x++){
				output.write((char *) &buffer[x][y], sizeof(pixel));
			}
			if((LAB_SIZE*4+1)*3 % 4 != 0){
				output.write((char *) &filler, (4-((LAB_SIZE*4+1)*3%4))*sizeof(uint8_t));
			}
		}
	}
}

void print_path(std::string path, Tile labirynth[][LAB_SIZE]){

	BMPHeader header;
	uint16_t type = 0x4d42;	

	pixel buffer[LAB_SIZE*4+1][LAB_SIZE*4+1];
	pixel path_hue;
	uint8_t filler[3] = {0x69, 0x42, 0xff};

	for(int x = 0; x < LAB_SIZE*4+1; x++){
		for(int y = 0; y < LAB_SIZE*4+1; y++){
			buffer[x][y].R = 255;
			buffer[x][y].G = 255;
			buffer[x][y].B = 255;
		}
	}

	std::ofstream output;

	output.open(path, std::ios::binary | std::ios::out);
	if(output.good() == true){

		for(int x = 0; x < LAB_SIZE; x++){
			for(int y = 0; y < LAB_SIZE; y++){
				if(labirynth[x][y].N == true){draw_wall(buffer, 'N', x, y);}
				if(labirynth[x][y].E == true){draw_wall(buffer, 'E', x, y);}
				if(labirynth[x][y].S == true){draw_wall(buffer, 'S', x, y);}
				if(labirynth[x][y].W == true){draw_wall(buffer, 'W', x, y);}

				if(labirynth[x][y].step != 0 && labirynth[x][y].START != 1){
					path_hue = hue_rotation(labirynth[x][y].step*HUE_STEP);
					std::cout << x << ", " << y << ", st: " << labirynth[x][y].step << " | ";
					for(int i = 1; i < 4; i++){
						for(int j = 1; j < 4; j++){
							buffer[4*x+i][4*y+j] = path_hue;
						}
					}
				}
				
			}
		}

		output.write((char *) &type, sizeof(uint16_t));
		output.write((char *) &header, sizeof(BMPHeader));

		for(int y = 0; y < LAB_SIZE*4+1; y++){
			for(int x = 0; x < LAB_SIZE*4+1; x++){
				output.write((char *) &buffer[x][y], sizeof(pixel));
			}
			if((LAB_SIZE*4+1)*3 % 4 != 0){
				output.write((char *) &filler, (4-((LAB_SIZE*4+1)*3%4))*sizeof(uint8_t));
			}
		}
	}
}

void print_path_and_ignored (std::string path, Tile labirynth[][LAB_SIZE]){

	BMPHeader header;
	uint16_t type = 0x4d42;	

	pixel buffer[LAB_SIZE*4+1][LAB_SIZE*4+1];
	pixel path_hue;
	uint8_t filler[3] = {0x69, 0x42, 0xff};

	for(int x = 0; x < LAB_SIZE*4+1; x++){
		for(int y = 0; y < LAB_SIZE*4+1; y++){
			buffer[x][y].R = 255;
			buffer[x][y].G = 255;
			buffer[x][y].B = 255;
		}
	}

	std::ofstream output;

	output.open(path, std::ios::binary | std::ios::out);
	if(output.good() == true){

		for(int x = 0; x < LAB_SIZE; x++){
			for(int y = 0; y < LAB_SIZE; y++){
				if(labirynth[x][y].N == true){draw_wall(buffer, 'N', x, y);}
				if(labirynth[x][y].E == true){draw_wall(buffer, 'E', x, y);}
				if(labirynth[x][y].S == true){draw_wall(buffer, 'S', x, y);}
				if(labirynth[x][y].W == true){draw_wall(buffer, 'W', x, y);}

				if(labirynth[x][y].step != 0 && labirynth[x][y].START != 1){
					path_hue = hue_rotation(labirynth[x][y].step*HUE_STEP);
					std::cout << x << ", " << y << ", st: " << labirynth[x][y].step << " | ";
					for(int i = 1; i < 4; i++){
						for(int j = 1; j < 4; j++){
							buffer[4*x+i][4*y+j] = path_hue;
						}
					}
				}
				
				if(labirynth[x][y].IGNORED == true){
					path_hue.R = 100;
					path_hue.G = 100;
					path_hue.B = 100;

					for(int i = 1; i < 4; i += 2){
						for(int j = 1; j < 4; j += 2){
							buffer[4*x+i][4*y+j] = path_hue;
						}
					}
				}
			}
		}

		output.write((char *) &type, sizeof(uint16_t));
		output.write((char *) &header, sizeof(BMPHeader));

		for(int y = 0; y < LAB_SIZE*4+1; y++){
			for(int x = 0; x < LAB_SIZE*4+1; x++){
				output.write((char *) &buffer[x][y], sizeof(pixel));
			}
			if((LAB_SIZE*4+1)*3 % 4 != 0){
				output.write((char *) &filler, (4-((LAB_SIZE*4+1)*3%4))*sizeof(uint8_t));
			}
		}
	}
}

#endif /* LABIRYNTH_OP_H */