Graphics 1 2D game template
===========================

You write your game in GameActivity.h and GameActivity.cpp. It will be a good idea to create
other modules/files as well, as if you put all your code in one module it could get out of hand.



Textures are loaded using the SOIL library. The source code of SOIL is included.





Game Activity
=============

Your rendering code goes in the render() method.

In the update() method, you step your game/simulation. The deltaT parameter is the amount of time passed since the last update.
When moving objects, you use code along the lines of:
	positionX += velocityX * deltaT
If you don't use deltaT, you will find that your game runs at different speeds on different machines, since the speed of your game
will be dependent on the frame rate. By using deltaT, you ensure that the simulation steps by the right amount given the time that has
elapsed since the last frame




Responding to user input (mouse, keyboard)
------------------------------------------

There are two ways of handling user input; event driven or polling.

Which approach you take depends on the user action that you are responding to. There are two kinds of action; discrete and continuous.

***DISCRETE ACTION; EVENT DRIVEN***

A discrete action occurs each time the user takes an action, such as shooting a bullet in response to a key press.xxxxx
You trigger these actions in response to events, using the event driven approach.
You implement this by adding code to one of the on*() methods (e.g. onMouseDown, onKeyDown, etc), for example:

void GameActivity::onKeyDown(int key)
{
	if (key == 'X')  // Respond to the X key
	{
		// Fire a bullet
		fireBullet();
	}
}

***CONTINUOUS ACTION; POLLING***

A continuous action occurs while the user holds a key, or in response to the mouse position. An example is moving
a character to the right while holding the RIGHT arrow key.
You handle these actions by polling the input state in the update() method. You poll the input state by
calling the methods defined by the InputState class (see InputState.h). Here they are for your convenience:
	bool isMouseButtonPressed(int button) const;			// Determine if a button is pressed; 0 = LEFT, 1 = MIDDLE, 2 = RIGHT
	int getMouseX() const;									// Get the mouse X and Y (below)
	int getMouseY() const;
	bool isKeyPressed(int key) const;						// Determine if a key is pressed

For example:

void GameActivity::update(double deltaT, double prevDeltaT)
{
	// ...
	// OTHER UPDATE CODE
	// ...

	// The input state can be access from the inputState member variable
	if (inputState->isKeyPressed(VK_RIGHT))
	{
		characterPositionX += CHARACTER_SPEED * deltaT;
	}

	// ...
	// OTHER UPDATE CODE
	// ...
}




Start and end screen
====================

The start and end screens are impelemented in the StartScreenActivity and EndScreenActivity classes.
You will want to change the start_screen.png and end_screen.png image files at least, to personalise them.
You can also modify StartScreenActivity and EndScreenActivity to alter their behaviour if you wish.
They work the same way as GameActivity




Adding new activities
=====================
If you need to implement new activities besides game, start screen and end screen, you will need to add member
variables for them in the OpenGLApplication class.
You will also need to construct and destroy them in the OpenGLApplication::initialise and OpenGLApplication::shutdown methods.


