#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include "raylib.h"
#include "../example_library/src/imgui.h"
#include "../example_library/src/rlImGui.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

int g_connectionLenLimit = 50;
float g_velocityRange = 1.0f;
int g_dotsAmount = 600;
bool g_showAreaGrid = false;
bool g_dotRadiusCollision = false;

class Dot;

std::vector<Dot> g_dots;


std::random_device rd;
std::default_random_engine eng(rd());

bool operator==(const Vector2 &lhs, const Vector2 &rhs){
	return lhs.x == rhs.x && lhs.y == rhs.y;
}


class Dot{
	public:
		Vector2 position;
		float radius;
		int curAreaIndexx = -1; 
		int curAreaIndexy = -1; 
		Dot(){
			position = Vector2{(float)GetRandomValue(0, SCREEN_WIDTH), (float)GetRandomValue(0,SCREEN_HEIGHT)};
			radius = (float)GetRandomValue(1, 4);
			SetRandomVelocity();
		}
		void Move(){
			position.x += xVel;
			position.y += yVel;
			BoundaryLimit();
		}
		void SetRandomVelocity(){
			std::uniform_real_distribution<float> distr(-g_velocityRange, g_velocityRange);
			xVel = distr(eng); 
			yVel = distr(eng);
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

void CreateDots(){
	for(int i = 0; i < g_dotsAmount; i++){
		g_dots.push_back(Dot());
	}
}


int main ()
{

	CreateDots();	

	// set up the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shattered dimension");

	SetTargetFPS(60);
	rlImGuiSetup(true);


	// game loop
	while (!WindowShouldClose())
	{
		// drawing
		BeginDrawing();
		ClearBackground(WHITE);
		rlImGuiBegin();

		ImGui::SliderInt("Connection length limit", &g_connectionLenLimit, 10,200);
		if(ImGui::SliderFloat("Velocity Range", &g_velocityRange, 0.5f, 10.0f)){
			for(int i = 0; i < g_dotsAmount; i++){
				g_dots[i].SetRandomVelocity();
			}
		}
		if(ImGui::SliderInt("Dots amount", &g_dotsAmount, 20, 800)){
			g_dots.clear();
			CreateDots();
		}
		if(ImGui::Checkbox("Show Area Grid", &g_showAreaGrid));
		if(ImGui::Checkbox("Dot radius collision *low performance*", &g_dotRadiusCollision));


		if(g_showAreaGrid){
			for(int i = 0; i < SCREEN_WIDTH; i+=g_connectionLenLimit){
				DrawLine(i, 0, i, SCREEN_HEIGHT, BLACK);
			}
			for(int i = 0; i < SCREEN_HEIGHT; i+=g_connectionLenLimit){
				DrawLine(0, i, SCREEN_WIDTH,i, BLACK);
			}
		}
		if(!g_dotRadiusCollision){
			for(int i = 0; i < g_dotsAmount; i++){
				g_dots[i].Move();
				DrawCircle(g_dots[i].position.x, g_dots[i].position.y, g_dots[i].radius, BLACK);
				int x = 0;
				int y = 0;

				// Coords in grid
				if((int)g_dots[i].position.x > 0){
					x = (int)g_dots[i].position.x * (SCREEN_WIDTH / g_connectionLenLimit) / SCREEN_WIDTH;
				}
				if((int)g_dots[i].position.y > 0){
					y = (int)g_dots[i].position.y * (SCREEN_WIDTH / g_connectionLenLimit) / SCREEN_WIDTH;
				}
				g_dots[i].curAreaIndexx = x;
				g_dots[i].curAreaIndexy = y;				

			}
			for(int i = 0; i < g_dotsAmount; i++){
				for(int j = 0; j < g_dotsAmount; j++){
					if(g_dots[i].curAreaIndexx != g_dots[j].curAreaIndexx || g_dots[i].curAreaIndexy != g_dots[j].curAreaIndexy || g_dots[i].position == g_dots[j].position)
						continue;
					if(CheckCollisionCircles(g_dots[i].position, g_connectionLenLimit, g_dots[j].position, g_dots[j].radius)){
						DrawLine(g_dots[i].position.x, g_dots[i].position.y, g_dots[j].position.x, g_dots[j].position.y, BLACK);
					}
				}
			}

		}

		
		rlImGuiEnd();
		EndDrawing();
	}

	// cleanup
	rlImGuiShutdown();
	CloseWindow();
	return 0;
}