//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

// Ensure that this file can only be included once
#pragma once

#include "InputState.h"


class OpenGLApplication;



/*
Activity base class

The application runs ONE activity at a time. Activities can be switched by calling OpenGLApplication::setCurrentActivity().

The activity receives events from the application when the user takes actions (mouse button presses, mouse motion, keyboard presses)


*/
class Activity
{
protected:
	OpenGLApplication *app;
	InputState *inputState;


public:
	Activity(OpenGLApplication *app);



	/*
	*
	* ACTIVITY METHODS
	*
	*/
	virtual void initialise();											// Initialise the activity at application start
	virtual void shutdown();											// Shutdown the activity at application close

	virtual void onSwitchIn();											// Activity switch in; called when the activity changes and this one switches in
	virtual void onSwitchOut();											// Activity switch in; called when the activity changes and this one switches out

	virtual void onReshape(int width, int height);						// Called when the window is resized
	virtual void update(double deltaT, double prevDeltaT);				// Update the application; if the current frame is frame number F, then the previous frame is F-1 and the one before that is F-2
																		// deltaT is the time elapsed from frame F-1 to frame F, prevDeltaT is the time elapsed from F-2 to F-1
	virtual void render();												// Render function

	// Event handler methods; these methods are called in response to user actions:
	virtual void onMouseDown(int button, int mouseX, int mouseY);		// Called when mouse button pressed
	virtual void onMouseUp(int button, int mouseX, int mouseY);			// Called when mouse button released
	virtual void onMouseMove(int mouseX, int mouseY);					// Called when mouse moved
	virtual void onKeyDown(int key);									// Called when key pressed
	virtual void onKeyUp(int key);										// Called when key released

};