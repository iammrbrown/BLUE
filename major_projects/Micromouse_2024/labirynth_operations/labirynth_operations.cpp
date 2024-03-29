#include <iostream>
#include <fstream>
#include <string>

/* 	In this file there is a set of functions and solutions related to operations on
	BMP files representing test labirynths for the Micromouse projects.				*/

/*Bro, a może po prostu napiszę to w C?*/

using namespace std;

void import_labirynth (string);

int main (int argc, char *argv[])

	return 0;
}

void import_labirynth (string path /*tablica?*/){
}

void print_bitmap (string path /*tablica*/){	
}

/* PORTED FROM THE DETECTOR CODE
	IT'S C, WON'T COMPILE xDDD		*/

	void drukuj_bitmape(short* norm_grid, gl_param p){
	FILE *bitmap = fopen(OUTPUT, "wb");

/*	TWORZENIE NAGŁÓWKA, KROK PO KROKU	*/
	uint16_t type = 0x4d42;	// "BM" - wedle formatu BMP
	BMPHeader header = {.offset				=54,
	/*	C zeruje	*/	.dib_header_size	=40,
	/*	wszystkie	*/	.width_px			=p.BMAP_X,
	/*	niezdef.	*/	.height_px			=p.BMAP_Y,
	/*	wartości	*/	.num_planes			=1,
	/*	więc mogę	*/	.bits_per_pixel		=24,
	/*	tu skrócić	*/	.x_resolution_ppm	=100,
	/*	dużo kodu	*/	.y_resolution_ppm	=100};

/*	Definiowanie rozmiaru bufora pikseli i pliku wyjściowego	*/
	uint32_t grid_size = (p.BMAP_X*p.BMAP_Y)*sizeof(pixel);
	header.size += grid_size;

/*	zapisywanie informacji nagłówka do pliku	*/
	fwrite(&type, sizeof(uint16_t), 1, bitmap);
	fwrite(&header, sizeof(header), 1, bitmap);

/*	rezerwacja przestrzeni bitmapy.
	signal.echo.phase bitmapy przechowywane są w 
	jednowymiarowej tablicy i adresowane: 
			[x+HEIGHT*y]					*/

	pixel *pixelbuffer = malloc(grid_size);
	pixel curr_pixel;

/*	Zapis pikseli do bufora bitmapy	(białe tło, ilość niebieskiego mówi o wartości. CMYK)	*/
	for(int x = 0; x < p.BMAP_X; x++){
		for(int y = 0; y < p.BMAP_Y; y++){
			curr_pixel.R = 0;
			curr_pixel.G = 0;
			curr_pixel.B = norm_grid[x+y*p.BMAP_X];

			pixelbuffer[x+p.BMAP_X*y] = curr_pixel; // X + SIZEX*Y
		}
	}

/* 	sprzątanie po sobie 	*/
	fwrite(pixelbuffer, grid_size, 1, bitmap);
	putc(0, bitmap);
	free(pixelbuffer);
	fclose(bitmap);
}
