#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "raylib.h"

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600	
#define RECT_WIDTH		20
#define RECT_HEIGHT		80
#define BALL_RADIUS		10	
#define MOVEMENT_OFFSET	20
#define X_KEY			88
#define W_KEY			87

typedef struct {
	int coord_x;
	int coord_y;
	int score;
} Player;

typedef struct {
	int coord_x;
	int coord_y;
int velocity;
	int dir;
	int angle_x;
	int angle_y;
} Ball;

int draw_board(Player *player_left, Player *player_right, Ball *ball)
{
	// Players
	DrawRectangle(player_left->coord_x, player_left->coord_y, RECT_WIDTH, RECT_HEIGHT, WHITE);
	DrawRectangle(player_right->coord_x, player_right->coord_y, RECT_WIDTH, RECT_HEIGHT, WHITE);

	// Middle line
	//
	DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);

	// Ball
	//
	if(ball->coord_x > SCREEN_WIDTH) return 1;
	else if(ball->coord_y > SCREEN_HEIGHT) ball->angle_y = -(ball->angle_y);
	else if(ball->coord_y < 0) ball->angle_y = abs(ball->angle_y);
	else if(ball->coord_x < 0) return -1;
	DrawCircle(ball->coord_x, ball->coord_y, BALL_RADIUS, WHITE);

	// Check collisions
	//
	if((ball->coord_x + BALL_RADIUS >= player_right->coord_x && ball->coord_x + BALL_RADIUS <= player_right->coord_x + RECT_WIDTH) && (ball->coord_y >= player_right->coord_y && ball->coord_y <= player_right->coord_y + RECT_HEIGHT)) ball->angle_x = -(ball->angle_x);

	
	if((ball->coord_x - BALL_RADIUS >= player_left->coord_x && ball->coord_x - BALL_RADIUS <= player_left->coord_x + RECT_WIDTH) && (ball->coord_y >= player_left->coord_y && ball->coord_y <= player_left->coord_y + RECT_HEIGHT))
		ball->angle_x = abs(ball->angle_x);

	return 0;
}

void reset_ball(Ball *ball)
{
	const int rand_angleX = rand() % 10 + 10;
	const int rand_angleY = rand() % 10 + 10;
	
	ball->coord_x = (SCREEN_WIDTH / 2);
	ball->coord_y = (SCREEN_HEIGHT / 2);
	ball->angle_x = rand_angleX;
	ball->angle_y = rand_angleY;
}

int main(void)
{
	srand(time(NULL));
	int is_playing = 0;

	const int top_limit = MOVEMENT_OFFSET;
	const int bottom_limit = SCREEN_HEIGHT - (5 * MOVEMENT_OFFSET);
	Player player_left = {
		.coord_x = RECT_WIDTH,
		.coord_y = (SCREEN_HEIGHT / 2) - RECT_HEIGHT,
		.score = 0
	};
	Player player_right = {
		.coord_x = SCREEN_WIDTH - (RECT_WIDTH * 2),
		.coord_y = (SCREEN_HEIGHT / 2) - RECT_HEIGHT,
		.score = 0
	};
	Ball ball = {0};
	reset_ball(&ball);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong with Raylib");
	int cpt = 3;
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		if(cpt > -1) {
			DrawText(TextFormat("%d", cpt), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, LIGHTGRAY);
			WaitTime(1);
			cpt--;
			is_playing = 0;
		} else {
			if (IsKeyDown(KEY_DOWN) && player_right.coord_y <= bottom_limit) player_right.coord_y += MOVEMENT_OFFSET;
			if (IsKeyDown(KEY_UP) && player_right.coord_y >= top_limit) player_right.coord_y -= MOVEMENT_OFFSET;
			if(IsKeyDown(X_KEY) && player_left.coord_y <= bottom_limit) player_left.coord_y += MOVEMENT_OFFSET;
			if(IsKeyDown(W_KEY) && player_left.coord_y >= top_limit) player_left.coord_y -= MOVEMENT_OFFSET;

			ball.coord_x += ball.angle_x;
			ball.coord_y += ball.angle_y;
			is_playing = draw_board(&player_left, &player_right, &ball);
			DrawText("Player 1", 20, 20, 20, LIGHTGRAY);
			DrawText(TextFormat("%d", player_left.score), 60, 50, 20, LIGHTGRAY);
			DrawText("Player 2", SCREEN_WIDTH - 140, 20, 20, LIGHTGRAY);
			DrawText(TextFormat("%d", player_right.score), SCREEN_WIDTH - 100, 50, 20, LIGHTGRAY);
		}
		if(is_playing != 0) {
			switch (is_playing) {
				case -1:
					player_right.score++;
					break;
				case 1:
					player_left.score++;
					break;
				default:
					break;
			}
			is_playing = 0;
			cpt = 3;
			reset_ball(&ball);
		}
		EndDrawing();
		WaitTime(0.03);
	}
	CloseWindow();
	return 0;
}
