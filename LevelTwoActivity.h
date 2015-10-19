//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

// Ensure that this file can only be included once
#pragma once

#include "Activity.h"

#include <string>


// GAME ACTIVITY
// Renders a grid with a smiley face character.
// WASD keys move the smiley face
// ARROW keys move the camera/viewpoint


class LevelTwoActivity : public Activity
{
private:
	// GAME activity variables go here

	// Camera position
	double camX, camY;


	GLuint playerTextureID, backgroundTextureID, collisionTextureID,
		enemyTextureID, fountainTextureID, bullet1ID, tileTexture2ID, tileTexture1ID,
		bullet2ID, heartTexture, gunTexture, healthText, badgeTexture;
	double playerX, playerY;


	GLint freq2;
	GLuint texture1;
	GLuint texture2;


public:
	LevelTwoActivity(OpenGLApplication *app);

	/*
	*
	* GAME activity methods
	* You should put your game code in the implementations of these methods
	* See LevelTwoActivity.cpp
	*
	*/
	virtual void initialise();											// Called on application start up
	virtual void shutdown();											// Called on application shut down

	virtual void onSwitchIn();											// Activity switch in; called when the activity changes and this one switches in
	virtual void onReshape(int width, int height);						// called when the window is resized
	virtual void update(double deltaT, double prevDeltaT);				// Update the application; if the current frame is frame number F, then the previous frame is F-1 and the one before that is F-2
	// deltaT is the time elapsed from frame F-1 to frame F, prevDeltaT is the time elapsed from F-2 to F-1
	virtual void render();												// Render function


	virtual void onMouseDown(int button, int mouseX, int mouseY);		// Called when mouse button pressed
	virtual void onMouseUp(int button, int mouseX, int mouseY);			// Called when mouse button released
	virtual void onMouseMove(int mouseX, int mouseY);					// Called when mouse moved
	virtual void onKeyDown(int key);									// Called when key pressed
	virtual void onKeyUp(int key);										// Called when key released

	void addEnemy();
	void collectableCollisionCheck();
	void enemyOnEnemyCollisionCheck();
	void bulletsCollisionCheck();
	void renderHUD();
	void changePlayerSprite(int row, int column);
	void drawTiles();
	void readMapFile(std::string filename);
	void readScore();
	void writeScore();
	GLuint loadTexture(std::string filename);
	GLvoid glPrint(std::string fmt);				// Custom GL "Print" Routine
	GLuint	base;
	HDC hDc;
	GLvoid LevelTwoActivity::BuildFont(GLvoid);



};