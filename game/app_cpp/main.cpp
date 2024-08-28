#include <iostream>
#include <random>
#include "raylib.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800
#define VELOCITY_RANGE_MIN 0.05
#define VELOCITY_RANGE_MAX 0.3


std::random_device rd;
std::default_random_engine eng(rd());

class Dot{
	public:
		Vector2 position;
		float radius;
		Dot(){
			position = Vector2{(float)GetRandomValue(0, SCREEN_WIDTH), (float)GetRandomValue(0,SCREEN_HEIGHT)};
			std::uniform_real_distribution<float> distr(VELOCITY_RANGE_MIN, VELOCITY_RANGE_MAX);
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
	Dot dots[20];
	for(int i = 0; i < 20; i++){
		dots[i] = Dot();
	}
	// set up the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Connection Sim");

	SetTargetFPS(60);
	
	// game loop
	while (!WindowShouldClose())
	{
		// drawing
		BeginDrawing();
		ClearBackground(GRAY);

		for(int i = 0; i < 20; i++){
			dots[i].Move();
			DrawCircle(dots[i].position.x, dots[i].position.y, dots[i].radius, BLACK);
		}
		
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	return 0;
}