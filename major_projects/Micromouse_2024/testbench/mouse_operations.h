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
	bool IGNORED = false;
} Tile;

#endif

#ifndef LAB_SIZE
#define LAB_SIZE 16
#endif


#ifndef MOUSE
#define MOUSE

typedef struct{
	int x = 0;
	int y = 0;
	char front = 'N';
	char left = 'W';
	char right = 'E';
	char rear = 'S';
} Mouse;

#endif

#ifndef MOUSE_OP
#define MOUSE_OP

/* ALGORYTHM OPERATIONS */

bool check_walls (Tile labirynth[][LAB_SIZE], int x, int y, char side){
	switch(side){
	case 'N':
		return labirynth[x][y].N;
		break;
	case 'E':
		return labirynth[x][y].E;
		break;
	case 'S':
		return labirynth[x][y].S;
		break;	
	case 'W':
		return labirynth[x][y].W;
		break;
	default:
		std::cout << "Mamy problem: CHK_WALLS - BAD SIDE" << std::endl;
		return 0;
	}
}

int check_distant_walls (Tile labirynth[][LAB_SIZE], int x, int y, char side){
	int d = 0;

	switch(side){
	case 'N':
		while(labirynth[x][y+d].N != true){d++;}
		break;
	case 'E':
		while(labirynth[x+d][y].E != true){d++;}
		break;
	case 'S':
		while(labirynth[x][y-d].S != true){d++;}
		break;
	case 'W':
		while(labirynth[x-d][y].W != true){d++;}
		break;
	default:	
		std::cout << "Mamy problem: CHK_D_WALLS - BAD SIDE" << std::endl;
	}
	return d;
}

void discover_cell (Mouse mouse, Tile sensors[][LAB_SIZE], Tile internal[][LAB_SIZE]){
	int x = mouse.x;
	int y = mouse.y;

	internal[x][y].N = sensors[x][y].N;
	internal[x][y].E = sensors[x][y].E;
	internal[x][y].S = sensors[x][y].S;
	internal[x][y].W = sensors[x][y].W;
}

void set_standard_internal(Tile internal[][LAB_SIZE]){
	for(int x = 0; x < LAB_SIZE; x++){
		for(int y = 0; y < LAB_SIZE; y++){
			internal[x][y].N = false;
			internal[x][y].E = false;
			internal[x][y].S = false;
			internal[x][y].W = false;
		}
	}

	for(int x = 0; x < LAB_SIZE; x++){internal[x][0].S = true; internal[x][LAB_SIZE-1].N = true;}
	for(int y = 0; y < LAB_SIZE; y++){internal[0][y].W = true; internal[LAB_SIZE-1][y].E = true;}

	internal[0][0].START = true;

	internal[8][8].END = true;
	internal[8][7].END = true;
	internal[7][8].END = true;
	internal[7][7].END = true;
}

/* ROBOT OPERATIONS */

void turn_left(Mouse mouse){
	char holder = mouse.front;
	mouse.front = mouse.left;
	mouse.left = mouse.rear;
	mouse.rear = mouse.right;
	mouse.right = holder;
}

void turn_right(Mouse mouse){
	char holder = mouse.front;
	mouse.front = mouse.right;
	mouse.right = mouse.rear;
	mouse.rear = mouse.left;
	mouse.left = holder;
}

void go_ahead(Mouse mouse){
	switch(mouse.front){
	case 'N':
		mouse.y++;
		break;
	case 'E':
		mouse.x++;
		break;
	case 'S':
		mouse.y--;
		break;
	case 'W':
		mouse.x--;
		break;
	default:
		std::cout << "Mamy problem: GO_AHEAD - BAD FACING\n";
	}
}

#endif