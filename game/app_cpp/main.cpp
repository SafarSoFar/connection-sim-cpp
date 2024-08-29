#include <iostream>
#include <random>
#include "raylib.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800
#define VELOCITY_RANGE 2
#define CONNECTION_RAD 80
#define DOTS_AMOUNT 600


std::random_device rd;
std::default_random_engine eng(rd());

bool operator==(Vector2 lhs, Vector2 rhs){
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

class Dot{
	public:
		Vector2 position;
		float radius;
		Dot(){
			position = Vector2{(float)GetRandomValue(0, SCREEN_WIDTH), (float)GetRandomValue(0,SCREEN_HEIGHT)};
			std::uniform_real_distribution<float> distr(-VELOCITY_RANGE, VELOCITY_RANGE);
			xVel = distr(eng); 
			yVel = distr(eng);
			radius = (float)GetRandomValue(1, 4);
		}
		void Move(){
			position.x += xVel;
			position.y += yVel;
			BoundaryLimit();
		}
	private:
		float xVel;
		float yVel;
		void BoundaryLimit(){
			if(position.x >= SCREEN_WIDTH){
				position.x = 0;
				position.y = SCREEN_HEIGHT-position.y; 
			}
			else if(position.x <= 0){
				position.x = SCREEN_WIDTH;
				position.y = SCREEN_HEIGHT-position.y; 
			}
			if(position.y >= SCREEN_HEIGHT){
				position.y = 0;
				position.x = SCREEN_WIDTH-position.x;
			}
			else if(position.y <= 0){
				position.y = SCREEN_HEIGHT;
				position.x = SCREEN_WIDTH-position.x;
			}
		}

};

int main ()
{
	Dot dots[DOTS_AMOUNT];
	for(int i = 0; i < DOTS_AMOUNT; i++){
		dots[i] = Dot();
	}
	// set up the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Connection simulation");

	SetTargetFPS(60);
	
	// game loop
	while (!WindowShouldClose())
	{
		// drawing
		BeginDrawing();
		ClearBackground(WHITE);

		for(int i = 0; i < DOTS_AMOUNT; i++){
			dots[i].Move();
			DrawCircle(dots[i].position.x, dots[i].position.y, dots[i].radius, BLACK);
		}

		for(int i = 0; i < DOTS_AMOUNT; i++){
			for(int j = 0; j < DOTS_AMOUNT; j++){
				if(dots[i].position == dots[j].position)
					continue;
				if(CheckCollisionCircles(dots[i].position, CONNECTION_RAD, dots[j].position, dots[j].radius)){
					DrawLine(dots[i].position.x, dots[i].position.y, dots[j].position.x, dots[j].position.y, BLACK);
				}
			}
		}
		
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	return 0;
}