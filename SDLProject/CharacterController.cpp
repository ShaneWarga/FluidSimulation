#include "CharacterController.h"
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
	int coord = x + ((WindowWidth/ CellSize) * y);
	return coord;
}

void CharacterController::CreateWallStateUpdate( int index) {
	if (index < 0 || index>objects->size()) { return; }
	if (objects->at(index)) {
		objects->at(index)->state->STATE_0 = true;
		objects->at(index)->state->STATE_1 = false;
		objects->at(index)->state->DEATH = false;
	}
}

void CharacterController::CreateWaterStateUpdate(int index) {
	if (index < 0 || index>objects->size()) { return; }
	if (objects->at(index)) {
		objects->at(index)->state->STATE_0 = false;
		objects->at(index)->state->STATE_1 = true;
		objects->at(index)->state->DEATH = false;
	}
}

void CharacterController::ClearStateUpdate(int index) {
	if (index < 0 || index>objects->size()) { return; }
	if (objects->at(index)) {
		objects->at(index)->state->STATE_0 = false;
		objects->at(index)->state->STATE_1 = false;
		objects->at(index)->state->DEATH = true;
	}
}

//I am not constructing new cells only changing state from dead to either water or wall
void CharacterController::CreateWall(SDL_Event* e) {
	int index = get2DIndexCell(e->button.x / CellSize, e->button.y / CellSize);
	int up = get2DIndexCell(e->button.x / CellSize, (e->button.y / CellSize) - 1);
	int down = get2DIndexCell(e->button.x / CellSize, (e->button.y / CellSize) + 1);
	int left = get2DIndexCell((e->button.x/ CellSize)-1, e->button.y / CellSize);
	int right = get2DIndexCell((e->button.x/ CellSize)+1, e->button.y / CellSize);
	CreateWallStateUpdate(index);
	CreateWallStateUpdate(up);
	CreateWallStateUpdate(down);
	CreateWallStateUpdate(left);
	CreateWallStateUpdate(right);
}
void CharacterController::CreateWater(SDL_Event* e) {
	int index = get2DIndexCell(e->button.x / CellSize, e->button.y / CellSize);
	int up = get2DIndexCell(e->button.x / CellSize, (e->button.y / CellSize) - 1);
	int down = get2DIndexCell(e->button.x / CellSize, (e->button.y / CellSize) + 1);
	int left = get2DIndexCell((e->button.x / CellSize) - 1, e->button.y / CellSize);
	int right = get2DIndexCell((e->button.x / CellSize) + 1, e->button.y / CellSize);
	CreateWaterStateUpdate(index);
	CreateWaterStateUpdate(up);
	CreateWaterStateUpdate(down);
	CreateWaterStateUpdate(left);
	CreateWaterStateUpdate(right);
}
void CharacterController::Clear(SDL_Event* e) {
	int index = get2DIndexCell(e->button.x / CellSize, e->button.y / CellSize);
	int up = get2DIndexCell(e->button.x / CellSize, (e->button.y / CellSize) - 1);
	int down = get2DIndexCell(e->button.x / CellSize, (e->button.y / CellSize) + 1);
	int left = get2DIndexCell((e->button.x / CellSize) - 1, e->button.y / CellSize);
	int right = get2DIndexCell((e->button.x / CellSize) + 1, e->button.y / CellSize);
	ClearStateUpdate(index);
	ClearStateUpdate(up);
	ClearStateUpdate(down);
	ClearStateUpdate(left);
	ClearStateUpdate(right);
}
