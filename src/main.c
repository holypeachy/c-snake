#include <stdio.h>
#include <raylib.h>

#include "game.h"

int main(void)
{
    InitWindow(res, res, "snake");
	SetTargetFPS(60);

	nextTick = GetTime() + tickTime;
	snake_len = 1;
	snake_meat[0] = (Vector2){19,19};
	occupied[19][19] = true;
	newApplePos();
	
    while (!WindowShouldClose())
    {
        BeginDrawing();
		ClearBackground(BLACK);
		ProcessTick();
		DrawAssets();
		HandleInput();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void DrawAssets(){
	DrawFPS(3,3);
	Vector2 temp = {apple_pos.x * cell_size, apple_pos.y * cell_size};
	DrawRectangleV(temp, (Vector2){cell_size, cell_size}, apple_col);
	for (int i = 0; i < snake_len; i++){
		temp.x = snake_meat[i].x * cell_size;
		temp.y = snake_meat[i].y * cell_size;
		DrawRectangleV(temp, (Vector2){cell_size, cell_size}, snake_col);
	}
}

void HandleInput(){
	// TODO: invalid input checking
	if (IsKeyDown(KEY_LEFT)){
		dir.x = -1;
		dir.y = 0;
	} else if (IsKeyDown(KEY_RIGHT)){
		dir.x = 1;
		dir.y = 0;
	} else if (IsKeyDown(KEY_UP)){
		dir.x = 0;
		dir.y = -1;
	} else if (IsKeyDown(KEY_DOWN)){
		dir.x = 0;
		dir.y = 1;
	}
}

void ProcessTick(){
	if (GetTime() >= nextTick){
		Vector2* temp;
		Vector2 lastPos;
		Vector2 newPos;
		for (int i = 0; i < snake_len; i++){
			temp = &snake_meat[i];
			
			// unmark
			occupied[(int)temp->y][(int)temp->x] = false;
			// update pos
			if (i == 0){
				newPos = *temp;
				temp->x += dir.x;
				temp->y += dir.y;
			} else{
				newPos = *temp;
				temp->x = lastPos.x;
				temp->y = lastPos.y;
			}
			lastPos = newPos;
			
			// loop back
			if (temp->x == 40) temp->x = 0;
			else if (temp->x == -1) temp->x = 39;
			else if (temp->y == 40) temp->y = 0;
			else if (temp->y == -1) temp->y = 39;
			
			// mark
			occupied[(int)temp->y][(int)temp->x] = true;
		}
		checkCollision();
		// increase len
		if (increase_len > 0){
			snake_len++;
			snake_meat[snake_len - 1] = lastPos;
			increase_len--;
			// TODO: is game over?
		}
		nextTick = GetTime() + tickTime;
	}
}

void checkCollision(){
	if (snake_meat[0].x == apple_pos.x && snake_meat[0].y == apple_pos.y){
		increase_len++;
		newApplePos();
	}
	// TODO: check snake collision
	
}

void newApplePos(){
	while(true){
		Vector2 temp = {GetRandomValue(0,39), GetRandomValue(0, 39)};
		if (!occupied[(int)temp.y][(int)temp.x]){
			apple_pos = temp;
			break;
		}
	}
}
