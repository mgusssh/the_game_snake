#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define SCREEN_X 30
#define SCREEN_Y 10
enum direction
{
    STOP,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

void print_frame(int x_size, int y_size, int field[y_size][x_size])
{
    int i, j;
    printf("\t");
    for (i = 0; i < x_size + 1; i++){
        printf("-");
    }
    printf("\n");
    for (i = 1; i < y_size; i++){
        printf("\t|");
        for (j = 1; j < x_size; j++){
            if (field[i][j] > 0) 
                printf("*");
            else if (!field[i][j])
                printf(" ");
            else if (field[i][j] == -1) 
                printf("O");
        }
        printf("|");
        printf("\n");
    }
    printf("\t");
    for (i = 0; i < x_size + 1; i++){
        printf("-");
    }
    printf("\n");
}

void spawn_apple(int x_size, int y_size, int field[y_size][x_size])
{
    int x, y;
    while (1) { 
        x = rand() % (x_size - 1) + 1;
        y = rand() % (y_size - 1) + 1;

        if (field[y][x] == 0) {
            field[y][x] = -1;
			return;
        }
    }
}

int snake(int direction, int *head_pos_x, int *head_pos_y, int x_size, int y_size, int field[y_size][x_size], int *score)
{
    int i, j;
    int new_x = *head_pos_x;
    int new_y = *head_pos_y;

    switch (direction){
    case UP: new_y--;
        break;
    case RIGHT: new_x++;
        break;
    case DOWN: new_y++;
        break;
    case LEFT: new_x--; 
        break;
    case STOP:
        return 1;
    default:
        break;
    }
    
    if (field[new_y][new_x] == -1){
        (*score)++;
        field[*head_pos_y][*head_pos_x] = (*score);
        spawn_apple(x_size, y_size, field);
    }
    else if (field[new_y][new_x] > 1){
        return 0;
    }
    else 
    {
        for (i = 0; i < y_size; i++){
            for (j = 0; j < x_size; j++){
                if (field[i][j] > 0){
                    field[i][j]--;
                }
            }
        }
    }
	
    field[new_y][new_x] = field[*head_pos_y][*head_pos_x] + 1;
    *head_pos_x = new_x;
    *head_pos_y = new_y;
    return 1;
}

int game()
{
	clock_t start_time;
	int direction = STOP;
	int score = 0;
    int level = 1;
    char input;
    double speed = 0.15;
    
    int x_size = SCREEN_X;
    int y_size = SCREEN_Y;

    int head_position_x = x_size/2;
    int head_postion_y = y_size/2;

    int field[SCREEN_Y][SCREEN_X] = {0};
    field[head_postion_y][head_position_x] = 1;
    
    spawn_apple(x_size, y_size, field);
    while (1)
    {        
		system("cls");
		
        printf("\n\tLEVEL: %d", level);
        printf("\n\tSCORE: %d \n", score);
        print_frame(x_size, y_size, field);
        
        if (score >= level * 3) {
            level++;
            if (level % 3 == 0) { 
                speed *= 0.8; 
            }
        }
        else if (level == 10) {
            printf("\n\tYOU WIN!\n");
            printf("\n\tPress any key to return to the menu...");
            while (1) {
                if (kbhit()) {
                    input = _getch();
                    if (input != 'a' && input != 's' && input != 'd' && input != 'w') {
                        return;
                    }
                }
            }
        }
        
        start_time = clock();
        while (((double)(clock() - start_time)) / CLOCKS_PER_SEC < speed) {
            if (kbhit()) {
            	input = _getch();
            }
        }
        
		if (input == 'p') { 
        	printf("\n\tThe game is on pause.\n\n"
				   "\tTo continue, click on 'c'\n"
				   "\tTo exit, click on 'e'\n");
        	while (1) {
            if (kbhit()) {
                input = _getch();
                if (input == 'c') { 
                    break;
                	}
                else if (input = 'e'){
                	return;
					}
            	}
        	}
    	}
		if (input == 'a' && direction != RIGHT)
            direction = LEFT;
        if (input == 'w' && direction != DOWN) 
            direction = UP;
        if (input == 's' && direction != UP)
            direction = DOWN;
        if (input == 'd' && direction != LEFT)
            direction = RIGHT;
            
        if (!snake(direction, &head_position_x, &head_postion_y, x_size, y_size, field, &score) ||
		    (head_position_x == 0 || head_position_x == x_size || head_postion_y == 0 || head_postion_y == y_size)){
        	printf("\n\tGAME OVER\n");
        	printf("\n\tPress any key to return to the menu...");
			while (1) {
                if (kbhit()) {
                    input = _getch();
                    if (input != 'a' && input != 's' && input != 'd' && input != 'w') {
                        return;
                    }
                }
            }
		}  
    }
}

void print_rules() {
    printf("\n\tRULES: \n\n");
    printf("\t1. Use arrow keys to control the snake:\n"
		   "\t\t w - up\n"
		   "\t\t s - down\n"
		   "\t\t a - left\n"
		   "\t\t d - right\n"
		   "\t\t p - stop game\n"
		   "\t2. Eat the food to grow longer\n"
   		   "\t3. Avoid hitting the walls or the snake's own body\n\n");
   	
   	printf("\n\tPress any key to start the game...");
    _getch(); 
}

int main()
{
    int key;
    
	do{
		system("cls");

		printf("\n\tTHE SNAKE GAME\n\n");
        printf("\t1. Start game\n"
        	   "\t2. Viewing rules\n"
               "\t0. Exit\n\n");
		
		while (printf("\t>> "), fflush(stdin),
			scanf("%d", &key) != 1 || !(key >= 0 || key <= 2))
		printf("Unknown command\n");
	
        switch (key){
        	case 1:
            	system("cls");
            	game(); 
            break;
            
            case 2:
            	system("cls");
        		print_rules();
            break;
            
        	case 0:
            	return 0;
            break;
        }
    }
	while (key);
		
	system("pause");
    return 0;
}
