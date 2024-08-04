#include "fxlib.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH  21
#define HEIGHT 8
#define SNAKE_MAX_LENGTH 50

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

typedef struct {
    int x;
    int y;
} Point;

Point snake[SNAKE_MAX_LENGTH];
int snake_length = 5;
int direction = RIGHT;

Point food;
int food_exists = 0;

void InitializeGame();
void Draw();
void Update();
void PlaceFood();
int CheckCollision();
void GameOver();


#define GETCODE(c1,c2) (gcode1=(c1),gcode2=(c2))
int Bkey_n_IsKeyDown(int keycode) {
    int kcode1 = 0, kcode2 = 0;
    int gcode1, gcode2;
    short unused = 0;

    switch(keycode) {
        case KEY_CTRL_UP: GETCODE(2,9); break;
        case KEY_CTRL_DOWN: GETCODE(3,8); break;
        case KEY_CTRL_LEFT: GETCODE(3,9); break;
        case KEY_CTRL_RIGHT: GETCODE(2,8); break;
        default: return 0;
    }

    if (Bkey_GetKeyWait(&kcode1, &kcode2, KEYWAIT_HALTOFF_TIMEROFF, 0, 0, &unused) == KEYREP_KEYEVENT) {
        if ((kcode1 == gcode1) && (kcode2 == gcode2)) {
            return 1;
        }
    }

    return 0;
}

int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int key;

    Bdisp_AllClr_DDVRAM();
    InitializeGame();
    Draw();

    while (1) {
        // Check for key press and update direction accordingly
        if (Bkey_n_IsKeyDown(KEY_CTRL_UP) && direction != DOWN) direction = UP;
        if (Bkey_n_IsKeyDown(KEY_CTRL_DOWN) && direction != UP) direction = DOWN;
        if (Bkey_n_IsKeyDown(KEY_CTRL_LEFT) && direction != RIGHT) direction = LEFT;
        if (Bkey_n_IsKeyDown(KEY_CTRL_RIGHT) && direction != LEFT) direction = RIGHT;

        Update();
        Draw();
        if (CheckCollision()) {
            GameOver();
            break;
        }
        Sleep(200); // Delay for game speed control
    }

    return 1;
}

void InitializeGame() {
    int i;
    for (i = 0; i < snake_length; ++i) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }
    direction = RIGHT;
    food_exists = 0;
}

void Draw() {
    int i;
    Bdisp_AllClr_DDVRAM();

    for (i = 0; i < snake_length; ++i) {
        locate(snake[i].x + 1, snake[i].y + 1);
        Print((unsigned char *)"*");
    }

    if (food_exists) {
        locate(food.x + 1, food.y + 1);
        Print((unsigned char *)"#");
    }

    Bdisp_PutDisp_DD();
}

void Update() {
    int i;
    Point new_head = snake[0];
    switch (direction) {
        case UP: new_head.y--; break;
        case DOWN: new_head.y++; break;
        case LEFT: new_head.x--; break;
        case RIGHT: new_head.x++; break;
    }

    // Handle screen wrapping
    if (new_head.x < 0) new_head.x = WIDTH - 1;
    if (new_head.x >= WIDTH) new_head.x = 0;
    if (new_head.y < 0) new_head.y = HEIGHT - 1;
    if (new_head.y >= HEIGHT) new_head.y = 0;

    // Move the snake body
    for (i = snake_length; i > 0; --i) {
        snake[i] = snake[i - 1];
    }
    snake[0] = new_head;

    // Place food if it doesn't exist
    if (!food_exists) {
        PlaceFood();
    }

    // Check if the snake has eaten the food
    if (snake[0].x == food.x && snake[0].y == food.y) {
        Point old_tail = snake[snake_length - 1];
        snake_length++;
        if (snake_length < SNAKE_MAX_LENGTH) {
            food_exists = 0;
            PlaceFood();
        }
        snake[snake_length - 1] = old_tail;
    }
}

void PlaceFood() {
    int i, valid;
    do {
        valid = 1;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        for (i = 0; i < snake_length; ++i) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
    food_exists = 1;
}

int CheckCollision() {
    int i;
    for (i = 1; i < snake_length; ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return 1;
        }
    }
    return 0;
}

void GameOver() {
    Bdisp_AllClr_DDVRAM();
    locate(5, 4);
    Print((unsigned char *)"Game Over");
    Bdisp_PutDisp_DD();
    Sleep(2000);
  
   
}

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section

#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum) {
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
#pragma section
