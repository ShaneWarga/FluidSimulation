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

std::thread Game::DefineNeighbors(int start, int end) {
	return std::thread([this, start, end] {
		for (int i = start; i < end; i++) {
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
				catch (const std::out_of_range& oor) {}
			}
		}
	});
}

void Game::CreateObjects() {
	controller = std::unique_ptr<CharacterController>(new CharacterController((&gameObjects)));
	inputManager->AddInputKey(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT, *controller->createWater);
	inputManager->AddInputKey(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_RIGHT, *controller->createWall);
	inputManager->AddInputKey(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_MIDDLE, *controller->clear);

	for (int j = 0; j < WindowHeight / CellSize; j++) {
		for (int i = 0; i < WindowWidth / CellSize; i++) {
			std::unique_ptr<Vector2> initPos = std::unique_ptr<Vector2>(new Vector2());
			initPos->x = i * CellSize;
			initPos->y = j * CellSize;
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
	int threadTotal = std::thread::hardware_concurrency();
	for (int threadCount = 0; threadCount < threadTotal; threadCount++) {
		neighborThreads.push_back(DefineNeighbors(
			(WindowWidth * threadCount)/(CellSize*threadTotal),
			(WindowWidth * (threadCount + 1))/(CellSize * threadTotal)
		));
	}
	for (int i = 0; i < neighborThreads.size(); i++) {
		neighborThreads[i].join();
	}
	for (int i = 0; i < gameObjects.size(); i++) {
		randIndex.push_back(i);
	}
}

void Game::UpdateAll() {
	for (int i = 0; i < randIndex.size(); i++) {
		gameObjects[randIndex[i]]->Update();
	}
}

std::thread Game::RenderRegion(int start, int end) {
	return std::thread([this, start, end] {
		for (int i = start; i < end; i++) {
			std::shared_ptr<GameObject> currentCell;
			for (int j = 0; j < WindowHeight / CellSize; j++) {
				renderObjects->Push(gameObjects[get2DIndex(i, j, WindowWidth / CellSize)].get());
			}
		}
	});
}

void Game::PushAll() {
	std::vector<std::thread> threads;
	int threadTotal = 1;
	for (int threadCount = 0; threadCount < threadTotal; threadCount++) {
		threads.push_back(RenderRegion(
			(WindowWidth * threadCount) / (CellSize * threadTotal),
			(WindowWidth * (threadCount + 1)) / (CellSize * threadTotal)
		));
	}
	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}

void Game::GenerateOutput(double deltaTime) {
	updateCounter++;
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	if (gameObjects.empty()) { CreateObjects(); };
	if (!gameObjects.empty()) {
		while (true) {
			SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
			SDL_RenderClear(mRenderer);
			inputManager->PollEvent();
			std::random_shuffle(randIndex.begin(), randIndex.end());
			PushAll(); 
			renderObjects->RenderAll();
			UpdateAll();
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
	renderObjects = std::unique_ptr<RenderQueue>(new RenderQueue(mRenderer));
	
	return true;
}


