//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

/*
Activity base class implementation

For more information on the structure of the template, please see README.txt
*/

#include "OpenGLApplication.h"

#include "Activity.h"



Activity::Activity(OpenGLApplication *app)
{
	this->inputState = &app->getInputState();
	this->app = app;
}





/*
*
* ACTIVITY METHODS
*
*/
void Activity::initialise()
{
	// Initialise the activity at application start
}

void Activity::shutdown()
{
	// Shutdown the activity at application close
}

void Activity::onSwitchIn()
{
	// Activity switch in

	// If you activity needs to perform any actions when it switches in, do them here
}

void Activity::onSwitchOut()
{
	// Activity switch out

	// If you activity needs to perform any actions when it switches out, do them here
}


void Activity::onReshape(int width, int height)
{
	// If you need to do anything when the screen is resized, do it here
}

void Activity::update(double deltaT, double prevDeltaT)
{
	// Update the application; if the current frame is frame number F, then the previous frame is F-1 and the one before that is F-2
	// deltaT is the time elapsed from frame F-1 to frame F, prevDeltaT is the time elapsed from F-2 to F-1

	// Update the simulation here
	// TRY NOT TO DO ANY RENDERING HERE
	// step the simulation forward by the amount of time specified by deltaT
	//
	// If you need to do different things depending on whether or not keys are pressed, etc,
	// get the member variable inputState and use its isKeyPressed(), getMouseX(), getMouseY() and isMouseButtonPressed() methods
	// to determine the state of the keys
}

void Activity::render()
{
	// OpenGL render calls go in this method
	//
	// IMPORTANT:
	// DO NOT update the simulation (move objects, change their state in response to user input) in this method or methods
	// called by this method. These methods should ONLY render objects
	//

	// First:
	// set your projection matrix:
	// <here>

	// Second:
	// set your camera matrix:
	// <here>

	// Third:
	// Render your objects:
	// <here>


	// If you want to have any overlay/HUD/etc:
	// Set a projection matrix that does not scroll:
	// <here>
	
	// Now render your HUD:
	// <here>
}

void Activity::onMouseDown(int button, int mouseX, int mouseY)
{
	// This method will be invoked when a mouse button is pressed
	// button: 0=LEFT, 1=MIDDLE, 2=RIGHT
	// mouseX and mouseY: position
}

void Activity::onMouseUp(int button, int mouseX, int mouseY)
{
	// This method will be invoked when a mouse button is released
	// button: 0=LEFT, 1=MIDDLE, 2=RIGHT
	// mouseX and mouseY: position
}

void Activity::onMouseMove(int mouseX, int mouseY)
{
	// This method will be invoked when the mouse is moved
	// mouseX and mouseY: position
}

void Activity::onKeyDown(int key)
{
	// This method will be invoked when a key is pressed
}

void Activity::onKeyUp(int key)
{
	// This method will be invoked when a key is released
}
