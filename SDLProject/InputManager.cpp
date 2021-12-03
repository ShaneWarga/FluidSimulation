#include "InputManager.h"

InputManager::InputManager() {
	keyDownInputMap = std::unique_ptr<std::map<SDL_Keycode, std::function<void(SDL_Event*)>>>(
		new std::map<SDL_Keycode, std::function<void(SDL_Event*)>>());
	MouseInputMap = std::unique_ptr<std::map<SDL_Keycode, std::function<void(SDL_Event*)>>>(
		new std::map<SDL_Keycode, std::function<void(SDL_Event*)>>());
	for (int i = 0; i < KEYBOARD_KEYS; i++) {
		Keys[i] = false;
	}
	for (int i = 0; i < MOUSE_KEYS; i++) {
		MouseKeys[i] = false;
	}
}

void InputManager::AddInputKey(SDL_EventType eventType, SDL_Keycode key, std::function<void(SDL_Event*)> f) {
	switch (eventType)
	{
	case SDL_KEYDOWN:
		keyDownInputMap->insert(std::pair<SDL_Keycode, std::function<void(SDL_Event*)>>(key, f));
		break;
	case SDL_MOUSEBUTTONDOWN:
		MouseInputMap->insert(std::pair<SDL_Keycode, std::function<void(SDL_Event*)>>(key, f));
		break;
	default:
		break;
	}
}

void InputManager::RemoveInputKey(SDL_EventType eventType, SDL_Keycode key) {
	switch (eventType)
	{
	case SDL_KEYDOWN:
		if (keyDownInputMap->at(key)) {
			keyDownInputMap->erase(key);
		}
	case SDL_MOUSEBUTTONDOWN:
		if (MouseInputMap->at(key)) {
			MouseInputMap->erase(key);
		}
	default:
		break;
	}
	
}
 
void InputManager::ReplaceInputKey(SDL_EventType eventType, SDL_Keycode key, std::function<void(SDL_Event*)> f) {
	switch (eventType)
	{
	case SDL_KEYDOWN:
		RemoveInputKey(eventType, key);
		AddInputKey(eventType, key, f);

	case SDL_MOUSEBUTTONDOWN:
		RemoveInputKey(eventType, key);
		AddInputKey(eventType, key, f);
	default:
		break;
	}
	
}

void InputManager::HandleKeyEvent(SDL_Event* event) {
	for (auto const& keyDown : *keyDownInputMap.get()) {
		if (Keys[keyDown.first]) {
			keyDownInputMap->at(keyDown.first)(event);
		}
	}
}

void InputManager::HandleMouseEvent(SDL_Event* event) {
	for (auto const& keyDown : *MouseInputMap.get()) {
		if (MouseKeys[keyDown.first]) {
			MouseInputMap->at(keyDown.first)(event);
		}
	}
}

void InputManager::SetControlledGameObject(std::shared_ptr<GameObject> object) {
	controlledObject = object;
}

void InputManager::PollEvent() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
		case SDL_KEYDOWN:
			Keys[e.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			Keys[e.key.keysym.sym] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseKeys[e.button.button] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			MouseKeys[e.button.button] = false;
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				SDL_Quit();
				exit(3);
			default:
				break;
			}
		default:
			break;
		}
	}
	HandleKeyEvent(&e);
	HandleMouseEvent(&e);
}