//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

#include "InputState.h"



InputState::InputState()
{
	// Initialise mouse button and key states
	for (int i = 0; i < _INPUTSTATE_NUM_MOUSEBUTTONS; i++)
	{
		mouseButtonStates[i] = false;
	}

	// Initialise mouse position
	mouseX = mouseY = 0;

	// Initialise key states
	for (int i = 0; i < _INPUTSTATE_NUM_KEYS; i++)
	{
		keys[i] = false;
	}
}


bool InputState::isMouseButtonPressed(int button) const
{
	return mouseButtonStates[button];
}

int InputState::getMouseX() const
{
	return mouseX;
}

int InputState::getMouseY() const
{
	return mouseY;
}

bool InputState::isKeyPressed(int key) const
{
	return keys[key];
}






// These methods are called by Win32 code to inform the InputState of user actions
void InputState::handleMouseDown(int button, int mouseX, int mouseY)
{
	mouseButtonStates[button] = true;
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

void InputState::handleMouseUp(int button, int mouseX, int mouseY)
{
	mouseButtonStates[button] = false;
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

void InputState::handleMouseMove(int mouseX, int mouseY)
{
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

void InputState::handleKeyDown(int key)
{
	keys[key] = true;
}

void InputState::handleKeyUp(int key)
{
	keys[key] = false;
}

