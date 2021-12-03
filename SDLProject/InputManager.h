#pragma once
#include "GameObject.h"
#include <SDL.h>
#include <functional>
#include <map>
#include <memory>
#define KEYBOARD_KEYS 332
#define MOUSE_KEYS 5


class InputManager
{
private:
	std::unique_ptr<std::map<SDL_Keycode, std::function<void(SDL_Event*)>>> keyDownInputMap;
	std::unique_ptr<std::map<SDL_Keycode, std::function<void(SDL_Event*)>>> MouseInputMap;
	std::shared_ptr<GameObject> controlledObject;
	bool Keys[332];
	bool MouseKeys[5];
public:
	InputManager();
	void AddInputKey(SDL_EventType eventType, SDL_Keycode key, std::function<void(SDL_Event*)> f);
	void RemoveInputKey(SDL_EventType eventType, SDL_Keycode key);
	void ReplaceInputKey(SDL_EventType eventType, SDL_Keycode key, std::function<void(SDL_Event*)> f);
	void SetControlledGameObject(std::shared_ptr<GameObject> object);
	void HandleKeyEvent(SDL_Event* event);
	void HandleMouseEvent(SDL_Event* event);
	void PollEvent();
};
