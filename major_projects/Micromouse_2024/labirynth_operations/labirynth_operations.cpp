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
} Tile;

void import_labirynth (std::string, Tile [][LAB_SIZE]);
void interpret_buffer (char [], char []); 

int main (int argc, char *argv[]){

	std::cout << "Hello, world! \n";
	

	std::cout << "Labirynth import testing \n";

	Tile labirynth[LAB_SIZE][LAB_SIZE];
	import_labirynth("./t_data/MM_TEST1.bmp", labirynth);

	for(int y = LAB_SIZE-1; y >= 0; y--){
		for(int x = 0; x < LAB_SIZE; x++){
			std::cout << labirynth[x][y].W << " ";
		}
		std::cout << std::endl;
	}

	return 0;
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
			labirynth[x][0].S = false;
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
					labirynth[x][y].E = true;
					break;
				default:
					labirynth[x][y].E = false;
				}
				if(x!=0){labirynth[x-1][y].W = labirynth[x][y].E;}
			}
			labirynth[LAB_SIZE-1][y].W = true;

			// TOP
			data_file.read(buffer, 20);
			interpret_buffer(buffer, row);

			for(int x = 0; x < LAB_SIZE; x++){
				switch(row[2*x+1]){
				case 'B':
					labirynth[x][y].N = true;
					break;
				default:
					labirynth[x][y].N = false;
				}
			}

		}

		data_file.close();

	}else std::cout << "The file bloody crashed! \n"; 
}

typedef struct{									
	uint32_t 	size;				// rozmiar pliku w bajtach				
	uint32_t	reserved; 			// można swobodnie wyzerować, nieważne w tym zastosowaniu
	uint32_t	offset;				// offset danych pikseli od początku pliku
	uint32_t	dib_header_size; 	// długość headera informacji o bitmapie
	int32_t 	width_px;			// szerokość bitmapy (w pikselach)
	int32_t 	height_px;			// wysokość  bitmapy (w pikselach)
	uint16_t	num_planes;			// coś skomplikowanego, związanego z ilością płaszczyzn kolorów. jedna dla wygody.
	uint16_t 	bits_per_pixel;		// ilość bitów przeznaczanych na każdy piksel.
	uint32_t	compression;		// 0 dla naszych zastosowań
	uint32_t	image_size_bytes; 	// przy compression = 0 ten parametr i tak jest ignorowany
	int32_t 	x_resolution_ppm; 	// znaczenie w druku - rozdzielczość w pikselach na metr X
	int32_t 	y_resolution_ppm; 	// znaczenie w druku - rozdzielczość w pikselach na metr X
	uint32_t	num_colors;			// po prawdzie - nie mam pojęcia
	uint32_t 	important_colors; 	// tu tak samo, ale działa
}BMPHeader;

typedef struct{
	uint8_t B;
	uint8_t G;
	uint8_t R;
}pixel;

void print_bitmap (std::string path /*VARIABLE BITMAP SIZE?*/){	
}

