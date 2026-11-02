#pragma once

const int res = 600;
const double cell_size = 15;
const int cell_row = res / cell_size; // 40

Vector2 snake_meat[1600] = {0};
int snake_len = 0;
int increase_len = 0;

bool occupied[40][40] = {false};

Color snake_col = GREEN;
Color snake_col2 = YELLOW;
Color snake_col3 = PINK;
Vector2 dir = {-1, 0};

double tickTime = 0.1;
double nextTick = 0;

Vector2 apple_pos = {10, 10};
Color apple_col = WHITE;

bool gameWon = false;
bool gameOver = false;
double nextResetTime = 0;
double timeToReset = 3;

Texture2D apple;
Sound collect;

void HandleInput();
void ProcessTick();
void DrawAssets();
void checkCollision();
void newApplePos();
void resetGame();
