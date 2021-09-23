#include "Game.h"




int get2DIndex(int x, int y, int division = 1) {
	int coord = x + (division * y);
	return coord;
}

Game::Game() {
	mWindow = nullptr;
	mRenderer = nullptr;
	mTicksCount = 0; 
	mIsRunning = true;
	playerPositionX = 0;
	playerPositionY = 0;
	inputManager = std::unique_ptr<InputManager>(new InputManager());
}



void Game::CreateObjects() {
	controller = std::unique_ptr<CharacterController>(new CharacterController((&gameObjects)));
	inputManager.get()->AddInputKey(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT, *controller.get()->createWater.get());
	inputManager.get()->AddInputKey(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_RIGHT, *controller.get()->createWall.get());
	inputManager.get()->AddInputKey(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_MIDDLE, *controller.get()->clear.get());

	for (int j = 0; j < WindowHeight / CellSize; j++) {
		for (int i = 0; i < WindowWidth / CellSize; i++) {
			std::unique_ptr<Vector2> initPos = std::unique_ptr<Vector2>(new Vector2());
			initPos.get()->x = i * CellSize;
			initPos.get()->y = j * CellSize;
			std::shared_ptr<GameObject> cell = std::shared_ptr<GameObject>(new Cell(std::move(initPos), CellSize));
			cell->SetRenderer(mRenderer);
			if(j > (WindowWidth / CellSize) / 2) {
				cell->state->DEATH = false;
				cell->state->STATE_0 = false;
				cell->state->STATE_1 = true;
				cell->state->STATE_2 = false;
				cell->state->INIT = false;
			}
			else {
				cell->state->DEATH = true;
				cell->state->STATE_0 = false;
				cell->state->STATE_1 = false;
				cell->state->STATE_2 = false;
				cell->state->INIT = false;
			}
			gameObjects.push_back(std::move(cell));
		}
	}
	std::vector<std::thread> neighborThreads;
	for (int i = 0; i < WindowWidth / CellSize; i++) {
		neighborThreads.push_back(std::thread([this, i] {
			std::shared_ptr<GameObject> currentCell;
			for (int j = 0; j < WindowHeight / CellSize; j++) {
				try {
					currentCell = gameObjects.at(get2DIndex(i, j, WindowWidth / CellSize));
				}
				catch (const std::out_of_range& oor) { return; }
				try {
					currentCell->neighbors.push_back(gameObjects.at(size_t(get2DIndex(i, j + 1, WindowWidth / CellSize))));
				}
				catch (const std::out_of_range& oor) {}
				try {
					currentCell->neighbors.push_back(gameObjects.at(size_t(get2DIndex(i - 1, j, WindowWidth / CellSize))));
				}
				catch (const std::out_of_range& oor) {}
				try {
					currentCell->neighbors.push_back(gameObjects.at(size_t(get2DIndex(i + 1, j, WindowWidth / CellSize))));
				}
				catch (const std::out_of_range& oor) {}}
			}));
	}
	for (int i = 0; i < neighborThreads.size(); i++) {
		neighborThreads[i].join();
	}
}



void Game::GenerateOutput(double deltaTime) {
	updateCounter++;
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	RenderQueue renderObjects(mRenderer);
	if (gameObjects.empty()) { CreateObjects(); };
	if (!gameObjects.empty()) {
		while (true) {
			SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
			SDL_RenderClear(mRenderer);
			inputManager.get()->PollEvent();
			for (int j = WindowHeight / CellSize - 1; j >= 0; j--) {
				for (int i = WindowWidth / CellSize - 1; i >= 0 ; i--) {
					gameObjects[get2DIndex(i, j, WindowWidth/CellSize)].get()->Update();
				}
				for (int i = WindowWidth / CellSize - 1; i >= 0; i--) {
					renderObjects.Push(gameObjects[get2DIndex(i, j, WindowWidth / CellSize)].get());
				}
			}
			renderObjects.RenderAll();
			SDL_UpdateWindowSurface(mWindow);
			SDL_Delay(deltaTime);
		}
	}else{return;}

}


bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	mWindow = SDL_CreateWindow("Cellular Automaton", 100, 100, WindowWidth, WindowHeight, 0);
	if (!mWindow) {
		SDL_Log("Failed to create window %s", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer %s", SDL_GetError());
		return false;
	}

	
	return true;
}


