#include <iostream>
#include <fstream>
#include <string>

#define BMAP_SIZE 256

#define LAB_SIZE 16
#define BMAP_BUFF 20

/* 	In this file there is a set of functions and solutions related to operations on
	BMP files representing test labirynths for the Micromouse projects.				*/

typedef struct {
	bool N;
	bool E;
	bool S;
	bool W;
	bool START = false;
	bool END = false;
	int step = 0;
} Tile;

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

void import_labirynth (std::string, Tile [][LAB_SIZE]);
void print_path (std::string, Tile [][LAB_SIZE]);
void interpret_buffer (char [], char []); 
void draw_wall (pixel [][LAB_SIZE*4+1], char, int, int);

void A_right_handed (Tile [][LAB_SIZE]);

int main (int argc, char *argv[]){

	std::cout << "Hello, world! \n";
	

	std::cout << "Labirynth import testing \n";

	Tile labirynth[LAB_SIZE][LAB_SIZE];
	import_labirynth("./t_data/MM_TEST1.bmp", labirynth);
 	
	A_right_handed(labirynth);

	print_path("./path.bmp", labirynth);

	return 0;
}

void A_right_handed (Tile labirynth[][LAB_SIZE]){
	char facing = 'N';
	char right = 'E';

	int x = 0;
	int y = 0;
	int step = 0;

	while(labirynth[x][y].END != true){
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
		if(step >= 256){step = 0;}

	}
}

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

void print_path(std::string path, Tile labirynth[][LAB_SIZE]){

	BMPHeader header;
	uint16_t type = 0x4d42;	

	pixel buffer[LAB_SIZE*4+1][LAB_SIZE*4+1];
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
				buffer[4*x+1][4*y+1].G = 255-labirynth[x][y].step*3; buffer[4*x+2][4*y+1].G = 255-labirynth[x][y].step*3; buffer[4*x+3][4*y+1].G = 255-labirynth[x][y].step*3;
				buffer[4*x+1][4*y+2].G = 255-labirynth[x][y].step*3; buffer[4*x+2][4*y+2].G = 255-labirynth[x][y].step*3; buffer[4*x+3][4*y+2].G = 255-labirynth[x][y].step*3;
				buffer[4*x+1][4*y+3].G = 255-labirynth[x][y].step*3; buffer[4*x+2][4*y+3].G = 255-labirynth[x][y].step*3; buffer[4*x+3][4*y+3].G = 255-labirynth[x][y].step*3;
			}
		}

		output.write((char *) &type, sizeof(uint16_t));
		output.write((char *) &header, sizeof(BMPHeader));
		std::cout << sizeof(BMPHeader) ;

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
