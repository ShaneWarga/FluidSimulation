#pragma once
#include "GameObject.h"
#include "CompileDirectives.h"
#include <functional>

class CharacterController
{
private:
	std::shared_ptr<GameObject> character;
	Vector2 velocity{ 0,0 };
	void BindMethods();
	std::vector<std::shared_ptr<GameObject>>* objects;

public:
	std::unique_ptr<std::function<void(SDL_Event* e)>> createWall;
	std::unique_ptr<std::function<void(SDL_Event* e)>> createWater;
	std::unique_ptr<std::function<void(SDL_Event* e)>> clear;
	CharacterController(std::vector<std::shared_ptr<GameObject>>* gameObjects);
	CharacterController(std::shared_ptr<GameObject> newCharacter);
	CharacterController(std::shared_ptr<GameObject> newCharacter, Vector2 initVelocity);
	Vector2 GetVelocity();
	void SetVelocity(Vector2 newVelocity);
	void CreateWall(SDL_Event* e);
	void CreateWater(SDL_Event* e);
	void CreateWallStateUpdate(int index);
	void CreateWaterStateUpdate(int index);
	void ClearStateUpdate(int index);
	void Clear(SDL_Event* e);

};

