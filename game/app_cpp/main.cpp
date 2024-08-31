#include <iostream>
#include <vector>
#include <set>
#include <random>
#include "raylib.h"
#include "../example_library/src/imgui.h"
#include "../example_library/src/rlImGui.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

int g_connectionLenLimitX = 50;
int g_connectionLenLimitY = 50;
bool g_syncLenLimit = true;

float g_velocityRange = 1.0f;
int g_dotsAmount = 600;
bool g_showAreaGrid = false;
bool g_isRelativeDotCollisionEnabled = false;
float g_RelativeDotRadiusCollision = 50.0f;

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


void RenderDebugMenu(){
	ImGui::Checkbox("Sync length limit", &g_syncLenLimit);
	if(ImGui::SliderInt("Connection length X", &g_connectionLenLimitX, 10,200)){
		if(g_syncLenLimit)
			g_connectionLenLimitY = g_connectionLenLimitX;
	}
	if(ImGui::SliderInt("Connection length  Y", &g_connectionLenLimitY, 10,200)){
		if(g_syncLenLimit)
			g_connectionLenLimitX = g_connectionLenLimitY;
	}

	if(ImGui::SliderFloat("Velocity Range", &g_velocityRange, 0.5f, 10.0f)){
		for(int i = 0; i < g_dotsAmount; i++){
			g_dots[i].SetRandomVelocity();
		}
	}
	if(ImGui::SliderInt("Dots amount", &g_dotsAmount, 20, 800)){
		g_dots.clear();
		CreateDots();
	}
	ImGui::Checkbox("Show Area Grid", &g_showAreaGrid);
	ImGui::Checkbox("Relative radius to dot collision enable *low performance*", &g_isRelativeDotCollisionEnabled);
	ImGui::SliderFloat("Relative dot collision radius", &g_RelativeDotRadiusCollision, 1.0f, 100.0f);

}

void ShowAreaGrid(){
	for(int i = 0; i < SCREEN_WIDTH; i+=g_connectionLenLimitX){
		DrawLine(i, 0, i, SCREEN_HEIGHT, BLACK);
	}
	for(int i = 0; i < SCREEN_HEIGHT; i+=g_connectionLenLimitY){
		DrawLine(0, i, SCREEN_WIDTH,i, BLACK);
	}	
}

void ControlDots(){
	for(int i = 0; i < g_dotsAmount; i++){
		g_dots[i].Move();
		DrawCircle(g_dots[i].position.x, g_dots[i].position.y, g_dots[i].radius, BLACK);
	}
}


void RenderAreaConnection(){
	// Getting every dot area
	for(int i = 0; i < g_dotsAmount; i++){
		int x = 0;
		int y = 0;

		// Coords in grid
		if((int)g_dots[i].position.x > 0){
			x = g_dots[i].position.x * (SCREEN_WIDTH / g_connectionLenLimitX) / SCREEN_WIDTH;
		}
		if((int)g_dots[i].position.y > 0){
			y = g_dots[i].position.y * (SCREEN_HEIGHT / g_connectionLenLimitY) / SCREEN_HEIGHT;
		}
		g_dots[i].curAreaIndexx = x;
		g_dots[i].curAreaIndexy = y;				
	}

	for(int i = 0; i < g_dotsAmount; i++){
		for(int j = 0; j < g_dotsAmount; j++){
			if(i == j || g_dots[i].curAreaIndexx != g_dots[j].curAreaIndexx || g_dots[i].curAreaIndexy != g_dots[j].curAreaIndexy)
				continue;
			DrawLine(g_dots[i].position.x, g_dots[i].position.y, g_dots[j].position.x, g_dots[j].position.y, BLACK);
		}
	}

}

void RenderRelativeDotRadius(){
	for(int i = 0; i < g_dotsAmount; i++){
		for(int j = 0; j < g_dotsAmount; j++){
			if(i == j)
				continue;
			if(CheckCollisionCircles(g_dots[i].position, g_RelativeDotRadiusCollision, g_dots[j].position, g_dots[j].radius)){
				DrawLineV(g_dots[i].position, g_dots[j].position, BLACK);
			}
		}
	} 
	//connectionIndices.clear();
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

		RenderDebugMenu();

		if(g_showAreaGrid){
			ShowAreaGrid();
		}

		ControlDots();

		if(g_isRelativeDotCollisionEnabled){
			RenderRelativeDotRadius();
		}
		else{
			RenderAreaConnection();
		}
	

		
		rlImGuiEnd();
		EndDrawing();
	}

	// cleanup
	rlImGuiShutdown();
	CloseWindow();
	return 0;
}