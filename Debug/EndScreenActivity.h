//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

#include <gl/gl.h>			// Header File For The OpenGL32 Library

#include "Activity.h"
#include <string>

// END SCREEN ACTIVITY
// displays the image end_screen.png. Quits when SPACE is pressed


class EndScreenActivity : public Activity
{
private:
	// We need a texture ID for our start screen texture
	GLuint textureID;



public:
	EndScreenActivity(OpenGLApplication *app);

	// ACTIVITY METHODS
	virtual void initialise();											// Called on application start up
	virtual void shutdown();											// Called on application shut down

	virtual void onSwitchIn();											// Activity switch in; called when the activity changes and this one switches in
	virtual void onReshape(int width, int height);						// called when the window is resized
	virtual void render();												// Render function
	GLvoid BuildFont(GLvoid);
	GLvoid glPrint(std::string fmt);
	void readScore();
	GLuint	base;
	HDC hDc;
	int score;
	virtual void onKeyUp(int key);										// Called when key released
};
