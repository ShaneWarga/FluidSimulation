#include "CharacterController.h"
#define CellDim 8
CharacterController::CharacterController(std::vector<std::shared_ptr<GameObject>>* gameObjects) {
	objects = gameObjects;
	BindMethods();
}


CharacterController::CharacterController(std::shared_ptr<GameObject> newCharacter) {
	character = newCharacter;
	BindMethods();
}

CharacterController::CharacterController(std::shared_ptr<GameObject> newCharacter, Vector2 initVelocity) {
	character = newCharacter;
	SetVelocity(initVelocity);
	BindMethods();
}

void CharacterController::BindMethods() {
	using namespace std::placeholders;
	createWall = std::unique_ptr<std::function<void(SDL_Event* e)>>(
		new std::function<void(SDL_Event* e)>(
			std::bind(&CharacterController::CreateWall, this, std::placeholders::_1)));
	createWater = std::unique_ptr<std::function<void(SDL_Event* e)>>(
		new std::function<void(SDL_Event* e)>(
			std::bind(&CharacterController::CreateWater, this, std::placeholders::_1)));
	clear = std::unique_ptr<std::function<void(SDL_Event* e)>>(
		new std::function<void(SDL_Event* e)>(
			std::bind(&CharacterController::Clear, this, std::placeholders::_1)));

}

Vector2 CharacterController::GetVelocity() {
	Vector2 temp = velocity;
	return temp;
}

void CharacterController::SetVelocity(Vector2 newVelocity) {
	velocity = newVelocity;
}

int get2DIndexCell(int x, int y) {
	int coord = x + ((1280/ CellDim) * y);
	return coord;
}

//I am not constructing new cells only changing state from dead to either water or wall
void CharacterController::CreateWall(SDL_Event* e) {
	int index = get2DIndexCell(e->button.x / CellDim, e->button.y / CellDim);
	if (index < 0 || index>70000) { return; }
	if (objects->at(index)) {
		for (auto neighbor : objects->at(index)->neighbors) {
			neighbor->state.get()->STATE_0 = true;
			neighbor->state.get()->STATE_1 = false;
			neighbor->state.get()->DEATH = false;
		}
		objects->at(index).get()->state.get()->STATE_0 = true;
		objects->at(index).get()->state.get()->STATE_1 = false;
		objects->at(index).get()->state.get()->DEATH = false;
	}
}
void CharacterController::CreateWater(SDL_Event* e) {
	int index = get2DIndexCell(e->button.x/CellDim, e->button.y/CellDim);
	if (index < 0 || index>70000) { return; }
	if (objects->at(index)) {
		for (auto neighbor : objects->at(index)->neighbors) {
			neighbor->state.get()->STATE_0 = false;
			neighbor->state.get()->STATE_1 = true;
			neighbor->state.get()->DEATH = false;
		}		
		objects->at(index).get()->state.get()->STATE_0 = false;
		objects->at(index).get()->state.get()->STATE_1 = true;
		objects->at(index).get()->state.get()->DEATH = false;
	}
}
void CharacterController::Clear(SDL_Event* e) {
	int index = get2DIndexCell(e->button.x / CellDim, e->button.y / CellDim);
	if (index < 0 || index>70000) { return; }
	if (objects->at(index)) {
		for (auto neighbor : objects->at(index)->neighbors) {
			neighbor->state.get()->STATE_0 = false;
			neighbor->state.get()->STATE_1 = false;
			neighbor->state.get()->DEATH = true;
		}
		objects->at(index).get()->state.get()->STATE_0 = false;
		objects->at(index).get()->state.get()->STATE_1 = false;
		objects->at(index).get()->state.get()->DEATH = true;
	}
}
