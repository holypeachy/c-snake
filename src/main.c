#include <stdio.h>
#include <raylib.h>

#include "game.h"

int main(void)
{
    InitWindow(res, res, "snake");
	SetTargetFPS(60);
	HideCursor();
	InitAudioDevice();
	
	apple = LoadTexture("assets/apple.png");
	SetTextureFilter(apple, TEXTURE_FILTER_POINT);
	collect = LoadSound("assets/collect.wav");

	resetGame();
	
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
    UnloadTexture(apple);
	UnloadSound(collect);
	CloseAudioDevice();

    return 0;
}

void DrawAssets(){
	// DrawFPS(3,3);
	Vector2 temp = {apple_pos.x * cell_size, apple_pos.y * cell_size};
	DrawRectangleV(temp, (Vector2){cell_size, cell_size}, apple_col);
	DrawTextureEx(apple, temp, 0, 1.0f, apple_col);
	for (int i = snake_len - 1; i >= 0; i--){
		temp.x = snake_meat[i].x * cell_size;
		temp.y = snake_meat[i].y * cell_size;
		DrawRectangleV(temp, (Vector2){cell_size, cell_size}, i == 0 ? snake_col : i % 2 == 1 ? snake_col2 : snake_col3);
	}
	if (gameWon){
		DrawText("YOU WON", 180, 250, 50, WHITE);
	} else if (gameOver){
		DrawText("GAME OVER", 150, 250, 50, RED);
	}
	if (gameWon || gameOver){
		char score[30];
		sprintf(score, "score: %d", snake_len + increase_len - 1);
		DrawText(score, 180, 350, 50, WHITE);
	}
}

void HandleInput(){
	Vector2 pos = snake_meat[0];
	Vector2 next = snake_meat[1];
	Vector2 diff = {0,0};
	bool pressed = false;
	if (snake_len != 1){
		diff.x = next.x - pos.x;
		diff.y = next.y - pos.y;
	}
	if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && diff.x != -1 && diff.x != 39){
		dir.x = -1;
		dir.y = 0;
		pressed = true;
	} else if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && diff.x != 1 && diff.x != -39){
		dir.x = 1;
		dir.y = 0;
		pressed = true;
	} else if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && diff.y != -1 && diff.y != 39){
		dir.x = 0;
		dir.y = -1;
		pressed = true;
	} else if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && diff.y != 1 && diff.y != -39){
		dir.x = 0;
		dir.y = 1;
		pressed = true;
	}
	
	if ((gameWon && pressed) || (gameOver && pressed)){
		resetGame();
	}
}

void ProcessTick(){
	if (GetTime() >= nextTick && !gameWon && !gameOver){
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
		// increase len
		if (increase_len > 0){
			snake_len++;
			snake_meat[snake_len - 1] = lastPos;
			increase_len--;
		}
		
		checkCollision();
		nextTick = GetTime() + tickTime;
	}
}

void checkCollision(){
	if (snake_meat[0].x == apple_pos.x && snake_meat[0].y == apple_pos.y){
		if (snake_len == 1599){
			gameWon = true;
			nextResetTime = GetTime() + timeToReset;
			return;
		}
		increase_len++;
		newApplePos();
		PlaySound(collect);
	}
	// TODO: check snake collision
	Vector2 head = snake_meat[0];
	Vector2 temp;
	for (int i = 1; i < snake_len; i++){
		temp = snake_meat[i];
		if (head.x == temp.x && head.y == temp.y){
			gameOver = true;
			nextResetTime = GetTime() + timeToReset;
			break;
		}
	}
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

void resetGame(){
	if (GetTime() >= nextResetTime) {
		nextTick = GetTime() + tickTime;
		snake_len = 1;
		snake_meat[0] = (Vector2){19,19};
		occupied[19][19] = true;
		newApplePos();
		gameOver = false;
		gameWon = false;
		nextResetTime = 0;
	}
}
