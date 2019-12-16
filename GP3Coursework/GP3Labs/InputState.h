#pragma once
#include "pch.h"
#include "Common.h"

struct InputState
{
	InputState()
	{
		keys.fill(false);
	}

	std::array<char, 400> keys;
	std::array<char, 8> axes;
	std::array<char, 5> buttons;
	glm::ivec2 mousePos;
	glm::ivec2 previousMousePos;
};
