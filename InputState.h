//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

#pragma once			// Ensures that this file can only be included once; do this in header files

// Defines InputState class; see InputState.cpp for implementation


// Define constants that we will need later
#define _INPUTSTATE_NUM_MOUSEBUTTONS 3
#define _INPUTSTATE_NUM_KEYS 256


/*
The InputState class represents the state of user input devices, namely the mouse
and keyboard. 

Within your code, call the getter methods to get the state of mouse buttons, the
mouse position or the keyboard states.

The Win32 application must invoke the handle*() methods to inform
InputState of user actions. This should already be done for you.
*/
class InputState
{
private:
	bool mouseButtonStates[_INPUTSTATE_NUM_MOUSEBUTTONS];	// Mouse button states: true for down, false for up
	int mouseX, mouseY;										// Mouse position
	bool keys[_INPUTSTATE_NUM_KEYS];						// Keyboard states; each member of the array corresponds to a key


public:
	InputState();


	// Getter methods; use these to poll the state of the mouse and keyboard
	bool isMouseButtonPressed(int button) const;			// Determine if a button is pressed; 0 = LEFT, 1 = MIDDLE, 2 = RIGHT
	int getMouseX() const;									// Get the mouse X and Y (below)
	int getMouseY() const;
	bool isKeyPressed(int key) const;						// Determine if a key is pressed
	


	//
	// !!! YOU CAN IGNORE THE METHODS BELOW !!!
	//
	// These methods are called by Win32 code to inform the InputState of user actions
	void handleMouseDown(int button, int mouseX, int mouseY);
	void handleMouseUp(int button, int mouseX, int mouseY);
	void handleMouseMove(int mouseX, int mouseY);
	void handleKeyDown(int key);
	void handleKeyUp(int key);
};

