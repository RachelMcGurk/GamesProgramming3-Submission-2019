#include "pch.h"
#include "Input.h"

Input* Input::m_instance = nullptr;

Input::Input()
{
}

Input* Input::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Input();
	}

	return m_instance;
}

void Input::SetKey(SDL_Keycode key, bool state)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}
	if (index < m_state.keys.size())
	{
		m_state.keys[index] = state;
	}
}

bool Input::GetKey(SDL_Keycode key)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}

	if (index < m_state.keys.size())
	{
		return m_state.keys[index];
	}
	else return false;
}

void Input::SetAxis(float axis, bool state)
{
	float index = axis;

	if (index < m_state.axes.size())
	{
		m_state.axes[index] = state;
	}
}

bool Input::GetAxis(float axis)
{
	float index = axis;

	if (index < m_state.axes.size())
	{
		return m_state.axes[index];
	}
	else return false;
}

void Input::SetButton(float button, bool state)
{
	float index = button;

	if (index < m_state.buttons.size())
	{
		m_state.buttons[index] = state;
	}
}

bool Input::GetButton(float button)
{
	float index = button;

	if (index < m_state.buttons.size())
	{
		return m_state.buttons[index];
	}
	else return false;
}

bool Input::GetKeyDown(SDL_Keycode key)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}

	if (index < m_state.keys.size())
	{
		if (m_previousState.keys[index])
		{
			return false;
		}
	}
	else return true;
}

bool Input::GetKeyUp(SDL_Keycode key)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}

	if (index < m_state.keys.size())
	{
		if (m_previousState.keys[index])
		{
			return true;
		}
	}
	else return false;
}

