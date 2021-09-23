#include "Cell.h"


// USER INPUT CHANGE TO RANDOM STATE ON CLICK
 

Cell::Cell(std::unique_ptr<Vector2> initialPosition, int size) {
	cell = std::unique_ptr<SDL_Rect>(new SDL_Rect());
	objectColor = std::unique_ptr<ColorData>(new ColorData());
	position = std::move(initialPosition);
	cell.get()->x = position.get()->x;
	cell.get()->y = position.get()->y;
	cell.get()->w = size;
	cell.get()->h = size;
}

Cell::~Cell() {
	delete position.get();
	delete objectColor.get();
	delete state.get();
	delete cell.get();
}


void Cell::Render() {
	SDL_SetRenderDrawColor(mRenderer, objectColor.get()->RED, objectColor.get()->GREEN, objectColor.get()->BLUE, objectColor.get()->ALPHA);
	SDL_RenderFillRect(mRenderer, cell.get());
}

void Cell::Update() {
	//if dead set to dead and return
	if (state.get()->DEATH) {
		SetColorData(0, 0, 0, 255);
		return;
	}
	//wall change to white then return
	if (state.get()->STATE_0) {
		SetColorData(255,255,255, 255);
		return;
	}
	/*water
	go down if possible
	go left or right if possible
	*/
	if (state.get()->STATE_1) {
		SetColorData(0, 0, 255, 255);
		for (auto neighbor : neighbors) {
			if (neighbor->state->DEATH) {
				neighbor->state.get()->STATE_1 = true;
				neighbor->state.get()->DEATH = false;
				state.get()->DEATH = true;
				state.get()->STATE_0 = false;
				return;
			}
		}
	}
}