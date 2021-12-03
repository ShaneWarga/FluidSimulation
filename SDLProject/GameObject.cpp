#include "GameObject.h"


GameObject& GameObject::SetRenderer(SDL_Renderer* renderer) {
	state = std::unique_ptr<CellStates>(new CellStates());
	state->INIT = true;
	state->STATE_0 = false;
	state->STATE_1 = false;
	state->STATE_2 = false;
	state->DEATH = false;
	mRenderer = renderer;
	return *this;
}

GameObject& GameObject::SetColorData(int red, int green, int blue, int alpha) {
	objectColor->RED = red;
	objectColor->GREEN = green;
	objectColor->BLUE = blue;
	objectColor->ALPHA = alpha;
	return *this;
}


void GameObject::SetPosition(Vector2 pos) {
	position->x = pos.x;
	position->y = pos.y;
	Update();
}

Vector2* GameObject::GetPosition() {
	return position.get();
}