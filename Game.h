#pragma once
#include "SDL.h"

struct vector2 {
	float x;
	float y;
};

class Game
{
public:
	
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	SDL_Window* mWindow;
	bool mIsRunning;
	SDL_Renderer* mRenderer;
	const int thickness = 15;
	vector2 mBallPos;
	vector2 mPaddlePos;
	vector2 mBallVel;
	Uint32 mTicksCount;
	int mPaddleDir;
	float deltaTime;
	
};

