//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

// Ensure that this file can only be included once
#pragma once

#include "InputState.h"
#include "Activity.h"


/*
OpenGLApplication delegates the application work to an Activity; see the Activity class, particularly the GameActivity class.

There is ONE activity that is currently running. 
*/
class OpenGLApplication
{
protected:
	InputState inputState;				// Input state; maintains state of input devices (mouse, keyboard)
	int screenWidth, screenHeight;		// Screen size
	Activity *currentActivity;			// The current activity; e.g. start screen, main game, end screen

private:
	bool finished;

public:
	Activity *startScreen, *game, *endScreen, *instructions, *levelTwo, *victory;

public:
	OpenGLApplication();


	// Getters for input state and screen size
	InputState & getInputState();
	const InputState & getInputState() const;
	int getScreenWidth() const;
	int getScreenHeight() const;
	double getAspectRatio() const;


	// Activity
	Activity * getCurrentActivity();
	void setCurrentActivity(Activity *a);


	// Initialise/shutdown application
	void initialise();
	void shutdown();
	
	// Finish
	void finish();						// Call to cause the application to close
	bool isApplicationFinished();		// Used by the main loop to see if we are done yet


	
	/*
	*
	* Methods called by Win32 code to inform our application that it must start up, update, render, reshape,
	* and of user actions; mouse down/up, mouse move, key down/up
	*
	* Mainly, they just pass on events to the current activity.
	*
	* You don't need to add to these methods.
	*
	*/
	void handleReshape(int width, int height);
	void handleUpdate(double deltaT, double prevDeltaT);
	void handleRender();

	// Methods called by Win32 code to inform our application of user actions
	void handleMouseDown(int button, int mouseX, int mouseY);
	void handleMouseUp(int button, int mouseX, int mouseY);
	void handleMouseMove(int mouseX, int mouseY);
	void handleKeyDown(int key);
	void handleKeyUp(int key);
};




