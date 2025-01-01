#include"game.h"

Game::Game() : mWindow(nullptr), mIsRunning(true),mRenderer(nullptr),mTicksCount(0) {}


bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Test", 100, 100, 1024, 768, 0);

	if (!mWindow) {
		SDL_Log("Could not initialize SDL %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	

	mBallPos.x = 512.0f;
	mBallPos.y = 384.0f;
	mPaddlePos.x = 7.5f;
	mPaddlePos.y = 384.0f;
	mBallVel.x = -300.0f;
	mBallVel.y = 350.0f;

	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);
	SDL_RenderClear(mRenderer);
	
	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		255,
		255,
		255
	);
	SDL_Rect wall{
		0,
		0,
		1024,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &wall);

	

	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 768;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x - thickness / 2),
		static_cast<int>(mPaddlePos.y - 60 / 2),
		thickness,
		60
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);
}

void Game::ProcessInput() 
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	mPaddleDir = 0;
	
	
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		if (mPaddlePos.y < thickness + 30) {
			mPaddlePos.y = 768.0f - thickness -30;
		}
		if (mPaddlePos.y > 768.0f - thickness - 30) {
			mPaddlePos.y = thickness + 30;
		}
	}
	mBallPos.y += mBallVel.y * deltaTime;
	mBallPos.x += mBallVel.x * deltaTime;
	if (mBallPos.y < thickness + thickness / 2) {
		mBallVel.y *= -1;
	}
	if (mBallPos.y > 768.0f - thickness - thickness / 2) {
		mBallVel.y *= -1;
	}
	if (mBallPos.x > 1024.0f - thickness - thickness / 2) {
		mBallVel.x *= -1;
	}

	float diff = mBallPos.y - mPaddlePos.y;
	if (diff < 0.0f) {
		diff *= -1;
	}
	if (diff <= 30 &&
		mBallPos.x <20.0f && mBallPos.x>10.0f &&
		mBallVel.x < 0.0f
		)
	{
		mBallVel.x *= -1;
	}
	if (mBallPos.x < 0.0f) {
		mIsRunning = false;
	}
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) ;

	deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
}
