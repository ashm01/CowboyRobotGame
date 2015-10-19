//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//
#include <gl/gl.h>			// Header File For The OpenGL32 Library

#include "Activity.h"


// START SCREEN ACTIVITY
// displays the image start_screen.png. Starts the game when SPACE is pressed

// See StartScreenActivity.cpp for implementation

class StartScreenActivity : public Activity
{
private:
	// We need a texture ID for our start screen texture
	GLuint textureID;



public:
	StartScreenActivity(OpenGLApplication *app);

	// ACTIVITY METHODS
	virtual void initialise();											// Called on application start up
	virtual void shutdown();											// Called on application shut down

	virtual void onSwitchIn();											// Activity switch in; called when the activity changes and this one switches in
	virtual void onReshape(int width, int height);						// called when the window is resized
	virtual void render();												// Render function

	virtual void onKeyUp(int key);										// Called when key released
};
