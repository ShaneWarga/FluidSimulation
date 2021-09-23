#pragma once
#include "GameObject.h"
#include"Rectangle.h"
#include "RenderQueue.h"
#include "Circle.h"
#include "Cell.h"
#include "InputManager.h"
#include "CharacterController.h"
#include "CompileDirectives.h"
#include <SDL.h>
#include <vector>
#include <memory>
#include <time.h>
#include <stdexcept>
#include <thread>

class Game
{
private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	int updateCounter = 0;
	int playerPositionX;
	int playerPositionY;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<CharacterController> controller;
	std::thread renderThread;

public:
	SDL_Window* mWindow;
	bool mIsRunning;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	Game();
	void GenerateOutput(double deltaTime);
	bool Initialize();
	void CreateObjects();
};

