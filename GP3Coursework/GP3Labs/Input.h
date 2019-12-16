#pragma once
#include "Common.h"
#include "InputState.h"
#define INPUT Input::GetInstance()

class Input
{
private:
	Input();
	static Input* m_instance;
	InputState m_state;
	InputState m_previousState;

public:
	static Input* GetInstance();
	//keyboard input
	void SetKey(SDL_Keycode key, bool state);
	bool GetKey(SDL_Keycode key); //check if it's currently down or not

	//joystick input
	void SetAxis(float axis, bool state);
	bool GetAxis(float axis);

	//controller button input
	void SetButton(float button, bool state);
	bool GetButton(float button);

	// handles mouse movement
	glm::ivec2 GetMousePos() { return m_state.mousePos; }
	void MoveMouse(glm::ivec2 delta) { m_state.previousMousePos = m_state.mousePos; m_state.mousePos += delta; }
	glm::ivec2 GetMouseDelta() { return m_state.mousePos - m_state.previousMousePos; }

	//checks for key up and key down
	bool GetKeyDown(SDL_Keycode key);
	bool GetKeyUp(SDL_Keycode key);
};