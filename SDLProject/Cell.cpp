#include "Cell.h"


// USER INPUT CHANGE TO RANDOM STATE ON CLICK
 

Cell::Cell(std::unique_ptr<Vector2> initialPosition, int size) {
	cell = std::unique_ptr<SDL_Rect>(new SDL_Rect());
	objectColor = std::unique_ptr<ColorData>(new ColorData());
	position = std::move(initialPosition);
	cell->x = position->x;
	cell->y = position->y;
	cell->w = size;
	cell->h = size;
}

Cell::~Cell() {
	delete position.get();
	delete objectColor.get();
	delete state.get();
	delete cell.get();
}


void Cell::Render() {
	SDL_SetRenderDrawColor(mRenderer, objectColor->RED, objectColor->GREEN, objectColor->BLUE, objectColor->ALPHA);
	SDL_RenderFillRect(mRenderer, cell.get());
}

void Cell::Update() {
	//if dead set to dead and return
	if (state->DEATH) {
		SetColorData(0, 0, 0, 255);
		return;
	}
	//wall change to white then return
	if (state->STATE_0) {
		SetColorData(255,255,255, 255);
		return;
	}
	/*water
	go down if possible
	go left or right if possible
	*/
	if (state->STATE_1) {
		SetColorData(0, 0, 255, 255);
		for (auto neighbor : neighbors) {
			if (neighbor->state->DEATH) {
				neighbor->state->STATE_1 = true;
				neighbor->state->DEATH = false;
				state->DEATH = true;
				state->STATE_0 = false;
				return;
			}
		}
	}
}