#pragma once
#include "GameObject.h"
#include <queue>
#include <SDL.h>
#include <mutex>

class RenderQueue
{
private:
	std::mutex renderMutex;
	std::queue<GameObject*> renderQueue;
	SDL_Renderer* mRenderer;

public:
	RenderQueue(SDL_Renderer* renderer);
	~RenderQueue();
	void Push(GameObject* object);
	void RenderOne();
	void RenderAll();
	void operator()();
	void Clear();
};

