//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

/*
LevelTwoActivity implementation


The comments in the file Activity.cpp give a little more info about each method

This activity is where the main game logic goes.
Most of your game code will go here
*/


#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "EnemyRobot.h"
#include "SOIL.h"
#include "Player.h"
#include "OpenGLApplication.h"			// Needed to access member functions and variables from OpenGLApplication
#include "LevelTwoActivity.h"
#include "GameActivity.h"
#include "Collectable.h"
#include <iostream>
#include <vector>
#include<sstream>
#include<fstream>
#include <cstdlib>     
#include <ctime> 
using std::vector;
std::string L2str;
vector <Collectable> L2collectables;
vector <EnemyRobot> L2enemies;
using namespace::std;
#define VIEW_SIZE 30.0	
#define MAP_SIZEX 100
#define MAP_SIZEY 100
GLuint L2tileTextures[10];
vector< vector<string> > L2map;  // the 2D array
double L2enemySpawnPoints[] = { 170, 30 };



double L2_CAMERA_MOVEMENT_SPEED = 10.0;
double L2_PLAYER_MOVEMENT_SPEED = 10.0;
int L2playerSpriteRow = 2;
int L2playerSpriteColumn = 1;
Player L2hero(196, 141);

double L2counter;


double L2playerDirection, L2tempY, L2tempX = 0;

double L2time, L2timer;
double L2limit = 0.1;
bool L2isShooting = false;
bool L2collision_flag = false;
int L2playerTileX, L2playerTileY = 0;




LevelTwoActivity::LevelTwoActivity(OpenGLApplication *app)
: Activity(app)		// Call super constructora
{
	// Initialse camera and player positions to be a the origin
	camX = 180;
	camY = 119;
	playerX = 198;
	playerY = 118;
	

}



/*
*
* ACTIVITY METHODS
* Put your application/game code here
*
*/
void LevelTwoActivity::initialise()
{
	// Initialise the activity; called at application start up
	HWND hwnd = GetForegroundWindow();
	hDc = ::GetDC(hwnd);
	// Load the player image as a texture using the SOIL library
	playerTextureID = loadTexture("Images/cowboy.png");
	backgroundTextureID = loadTexture("Images/backgroundMap.png");
	enemyTextureID = loadTexture("Images/enemyRobot.png");
	heartTexture = loadTexture("Images/heart.png");
	gunTexture = loadTexture("Images/gun.png");
	badgeTexture = loadTexture("Images/sheriff.png");
	bullet1ID = loadTexture("Images/bullet.png");
	bullet2ID = loadTexture("Images/bullet2.png");
	healthText = loadTexture("Images/Health.png");
	L2tileTextures[1] = loadTexture("Images/gravelTile.png");
	L2tileTextures[2] = loadTexture("Images/bridgeHorvTile.png");
	L2tileTextures[3] = loadTexture("Images/bridgeVertTile.png");
	L2tileTextures[4] = loadTexture("Images/rockTile.png");
	L2tileTextures[5] = loadTexture("Images/lavaTile.png");
	L2tileTextures[6] = loadTexture("Images/finishTile.png");
	
	readMapFile("level2.txt");
	
	//initialisng collectables
	srand(time(NULL));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, heartTexture, Collectable::Type::HEART));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, heartTexture, Collectable::Type::HEART));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, heartTexture, Collectable::Type::HEART));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, heartTexture, Collectable::Type::HEART));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, gunTexture, Collectable::Type::GUN));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, badgeTexture, Collectable::Type::BADGE));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, badgeTexture, Collectable::Type::BADGE));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, badgeTexture, Collectable::Type::BADGE));
	L2collectables.push_back(Collectable(rand() % 160 + 30, rand() % 170 + 30, badgeTexture, Collectable::Type::BADGE));
	
	AllocConsole();
	freopen("conout$", "w", stdout);
	BuildFont();
}


void LevelTwoActivity::shutdown()
{
	// Shutdown the activity; called at application finish

	// Delete the texture
	glDeleteTextures(1, &playerTextureID);
}



void LevelTwoActivity::onSwitchIn()
{
	// Activity switch in


	// EXAMPLE CODE
	glClearColor(0.0, 0.0, 0.0, 0.0);	//sets the clear colour to black
	readScore();
	
	PlaySound("Sounds/air_raid.wav", NULL, SND_ASYNC | SND_FILENAME);

}

void LevelTwoActivity::onReshape(int width, int height)
{
	// If you need to do anything when the screen is resized, do it here

	// EXAMPLE CODE
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	double aspect = app->getAspectRatio();
	// The height of the visible area is defined by VIEW_SIZE. Split it half each way around the origin, hence the *0.5
	// Take the aspect ratio into consideration when computing the width of the visible area
	gluOrtho2D(-VIEW_SIZE*0.5*aspect, VIEW_SIZE*0.5*aspect, -VIEW_SIZE*0.5, VIEW_SIZE*0.5);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void LevelTwoActivity::update(double deltaT, double prevDeltaT)
{

	L2time = deltaT;
	if (L2hero.playerHealth <= 0)
	{
		//player dies and game is ended
		app->game->shutdown();
		app->setCurrentActivity(app->endScreen);
	}
	addEnemy();

	L2tempX = playerX;
	L2tempY = playerY;
	//checking collision with heart
	//only checks when collectables are on map
	collectableCollisionCheck();
	enemyOnEnemyCollisionCheck();
	bulletsCollisionCheck();
	L2hero.update(playerX, playerY, L2playerSpriteRow, L2playerSpriteColumn, L2playerDirection);

	if (inputState->isKeyPressed(VK_LEFT))
	{
		if (camX> 20 && playerX<180)
			camX -= L2_CAMERA_MOVEMENT_SPEED * deltaT;
	}
	if (inputState->isKeyPressed(VK_RIGHT))
	{
		if (camX< 180 && playerX>20)
			camX += L2_CAMERA_MOVEMENT_SPEED * deltaT;
	}
	if (inputState->isKeyPressed(VK_UP))
	{
		if (camY< 185 && playerY>15)
			camY += L2_CAMERA_MOVEMENT_SPEED * deltaT;
	}
	if (inputState->isKeyPressed(VK_DOWN))
	{
		if (camY> 15 && playerY<185)
			camY -= L2_CAMERA_MOVEMENT_SPEED * deltaT;
	}

	// WASD control player
	//FIRST CHECKS TO SEE IF THE USER IS PRESSING DOWN TWO CONTROLS E.G. WS or AD etc

	if (inputState->isKeyPressed('S') && inputState->isKeyPressed('A'))
	{
		//s
		if (playerY > 2)
			playerY -= L2_PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY> 15 && playerY<185 && !L2collision_flag)
			camY -= L2_CAMERA_MOVEMENT_SPEED * deltaT;
		//a
		if (playerX > -1.5){
			playerX -= L2_PLAYER_MOVEMENT_SPEED * deltaT;
		}
		if (camX > 20 && playerX < 180 && !L2collision_flag)
			camX -= L2_CAMERA_MOVEMENT_SPEED * deltaT;

		changePlayerSprite(1, L2playerSpriteColumn);
		L2playerDirection = -45;


	}
	else if (inputState->isKeyPressed('S') && inputState->isKeyPressed('D'))
	{
		//s
		if (playerY > 2)
			playerY -= L2_PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY> 15 && playerY<185 && !L2collision_flag)
			camY -= L2_CAMERA_MOVEMENT_SPEED * deltaT;
		//d
		if (playerX < 199)
			playerX += L2_PLAYER_MOVEMENT_SPEED * deltaT;
		if (camX< 180 && playerX>20 && !L2collision_flag)
			camX += L2_CAMERA_MOVEMENT_SPEED * deltaT;
		changePlayerSprite(7, L2playerSpriteColumn);
		L2playerDirection = -135;
	}
	else if (inputState->isKeyPressed('W') == 1 && inputState->isKeyPressed('D') == 1)
	{
		//w
		if (playerY < 198)
			playerY += L2_PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY< 185 && playerY>15 && !L2collision_flag)
			camY += L2_CAMERA_MOVEMENT_SPEED * deltaT;
		//d
		if (playerX < 199)
			playerX += L2_PLAYER_MOVEMENT_SPEED * deltaT;
		if (camX< 180 && playerX>20 && !L2collision_flag)
			camX += L2_CAMERA_MOVEMENT_SPEED * deltaT;
		changePlayerSprite(5, L2playerSpriteColumn);
		L2playerDirection = 135;



	}
	else if (inputState->isKeyPressed('W') == 1 && inputState->isKeyPressed('A') == 1)
	{
		//w
		if (playerY < 198)
			playerY += L2_PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY< 185 && playerY>15 && !L2collision_flag)
			camY += L2_CAMERA_MOVEMENT_SPEED * deltaT;
		//a
		if (playerX > 1.5){
			playerX -= L2_PLAYER_MOVEMENT_SPEED * deltaT;
		}
		if (camX > 20 && playerX < 180 && !L2collision_flag)
			camX -= L2_CAMERA_MOVEMENT_SPEED * deltaT;
		changePlayerSprite(3, L2playerSpriteColumn);
		L2playerDirection = 45;



	}
	else{

		//NOW CHECKS FOR THE SINGLE USE OF WSAD CONTROLS
		if (inputState->isKeyPressed('A'))
		{
			if (playerX > 1.5){
				playerX -= L2_PLAYER_MOVEMENT_SPEED * deltaT;
			}
			if (camX > 20 && playerX < 180 && !L2collision_flag)
				camX -= L2_CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(2, L2playerSpriteColumn);
			L2playerDirection = 0;
		}
		if (inputState->isKeyPressed('D'))
		{
			if (playerX < 199)
				playerX += L2_PLAYER_MOVEMENT_SPEED * deltaT;
			if (camX< 179.5 && playerX>20 && !L2collision_flag)
				camX += L2_CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(6, L2playerSpriteColumn);
			L2playerDirection = 180;
		}

		if (inputState->isKeyPressed('W'))
		{
			if (playerY < 198)
				playerY += L2_PLAYER_MOVEMENT_SPEED * deltaT;
			if (camY< 184.5 && playerY>15 && !L2collision_flag)
				camY += L2_CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(4, L2playerSpriteColumn);
			L2playerDirection = 90;
		}

		if (inputState->isKeyPressed('S'))
		{
			if (playerY > 2)
				playerY -= L2_PLAYER_MOVEMENT_SPEED * deltaT;
			if (camY >15 && playerY < 185 && !L2collision_flag)
				camY -= L2_CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(0, L2playerSpriteColumn);
			L2playerDirection = -90;
		}



	}
	//###COLLISON DETECTION ON TILES####
	//determing the player offset for tile collison detetction according to the direction
	//i.e checking what tile is determined bythe direction the hero is facing
	cout << L2playerDirection<< endl;
	if (L2playerDirection == 0)
	{
		L2playerTileY = ceil((playerY - 3) / 2);//the game space is 200*200 however each tile is 2*2
		L2playerTileX = ceil((playerX - 2.5) / 2);// so dividing by 2 gives me the correect tile in the bitmap
	}

	else if (L2playerDirection == -135 || L2playerDirection == -90){
		L2playerTileX = ceil((playerX - 1.15) / 2);
		L2playerTileY = ceil((playerY - 4) / 2);
	}
	else if (L2playerDirection == 45) 
	{
		L2playerTileX = ceil((playerX - 2) / 2);
		L2playerTileY = ceil((playerY - 4) / 2);
	}
	else if (L2playerDirection == -45)
	{
		L2playerTileX = ceil((playerX - 3) / 2);
		L2playerTileY = ceil((playerY - 4) / 2);
	}
	else
	{
		L2playerTileX = ceil((playerX - 1.15) / 2);
		L2playerTileY = ceil((playerY - 3) / 2);
	}
	
	//had an error where the x tile would be over the range, this check fixed that
	if (L2playerTileY>99)
		L2playerTileY = 99;
	if (L2playerTileY<0)
		L2playerTileY = 0;
	if (L2playerTileX>99)
		L2playerTileX = 99;
	if (L2playerTileX<0)
		L2playerTileX =0;
	//getting the value of the tile within the bitmap of my map
	int tileValue = atoi(L2map[L2playerTileY][L2playerTileX].c_str());
	//Checking whether play has made it to the finish area with all the badges
	if (tileValue == 6 && L2hero.badgeCount == 4)
	{
		//ends the game
		app->setCurrentActivity(app->victory);
	}
	//if the tile is less then 3 i.e. grass or path no collision
	bool inLavaFlag;

	//slows the player down if he is in the long grass(tile 6)
	if (tileValue == 5)
	{
		L2collision_flag = false;
		inLavaFlag = true;
		L2hero.playerHealth -= 0.2;
	}
	else if (tileValue < 4 || tileValue == 6){
		L2collision_flag = false;
		inLavaFlag = false;
	}
	else
	{
		inLavaFlag = false;
		L2collision_flag = true;
		playerX = L2tempX;
		playerY = L2tempY;
	}
	//spacebar press
	if (inputState->isKeyPressed(32))
	{
		L2playerSpriteColumn = 11;

	}
	//using shift to speed charecter up
	if (inputState->isKeyPressed(VK_SHIFT) && L2hero.getCollisionFlag() == false && inLavaFlag == false)
	{
		L2_CAMERA_MOVEMENT_SPEED = 25.0;
		L2_PLAYER_MOVEMENT_SPEED = 25.0;
		L2limit = 0.05;
	}
	else if (L2hero.getCollisionFlag() == true)
	{
		L2_CAMERA_MOVEMENT_SPEED = 3.0;
		L2_PLAYER_MOVEMENT_SPEED = 3.0;
	}
	else if (inLavaFlag==true)
	{
		L2_CAMERA_MOVEMENT_SPEED = 5.0;
		L2_PLAYER_MOVEMENT_SPEED = 5.0;
	}
	else
	{
		L2_CAMERA_MOVEMENT_SPEED = 10.0;
		L2_PLAYER_MOVEMENT_SPEED = 10.0;
		L2limit = 0.1;
	}


}

void LevelTwoActivity::render()
{

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	// Use the negated camera position as a translation; effectively we move the world and the camera so that the camera is at 0,0,0
	glTranslated(-camX, -camY, 0.0);

	// Render the  background
	
	drawTiles();
	

	//hero.boundingBox.draw();
	L2hero.draw(playerTextureID);

	//drawing Collectables
	for each (Collectable  c in L2collectables)
	{
		if (c.type == Collectable::Type::GUN)
		{
			c.draw(2);
		}
		else
		{
			c.draw(1);
		}
	}

	//drawing each player bullet  
	for (int i = 0; i < L2hero.bullets.size(); i++)
	{
		//hero.bullets[i].boundingBox.draw();
		L2hero.bullets[i].draw(bullet1ID, 0.1);
	}
	//drawing each enemy in the vector
	for (int i = 0; i < L2enemies.size(); i++)
	{
		//enemies[i].boundingBox.draw();
		L2enemies[i].draw();
	}
	for (int i = 0; i < L2enemies.size(); i++)
	{
		for (int j = 0; j < L2enemies[i].bullets.size(); j++)
		{
			//enemies[i].bullets[j].boundingBox.draw();
			L2enemies[i].bullets[j].draw(bullet2ID, 0.3);
		}
	}
	renderHUD();
	glFlush();

}



void LevelTwoActivity::onMouseDown(int button, int mouseX, int mouseY)
{
	// This method will be invoked when a mouse button is pressed
	// button: 0=LEFT, 1=MIDDLE, 2=RIGHT
	// mouseX and mouseY: position
}

void LevelTwoActivity::onMouseUp(int button, int mouseX, int mouseY)
{
	L2hero.shoot(2);
	L2isShooting = true;
	L2playerSpriteColumn = 11;
	//PlaySound("Sounds/shoot.wav", NULL, SND_ASYNC | SND_FILENAME);
}

void LevelTwoActivity::onMouseMove(int mouseX, int mouseY)
{
	// This method will be invoked when the mouse is moved
	// mouseX and mouseY: position
}

void LevelTwoActivity::onKeyDown(int key)
{
	// This method will be invoked when a key is pressed
}

void LevelTwoActivity::onKeyUp(int key)										// Called when key released
{
	// Key released

	// Exit the start screen when the SPACE key is released, NOT pressed
	// That way the next activity starts with the space key NOT pressed
	if (key == VK_F1)
	{
		// F1; switch to end screen activity
		app->setCurrentActivity(app->endScreen);
	}

	if (key == VK_SPACE)
	{
		L2hero.shoot(2);
		L2isShooting = true;
		L2playerSpriteColumn = 11;
		PlaySound("Sounds/shoot.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
	if (key == VK_F12)
	{


	}
}

void LevelTwoActivity::changePlayerSprite(int row, int column)
{
	L2playerSpriteRow = row;
	L2playerSpriteColumn = column;
	if (L2counter > L2limit){
		if (L2playerSpriteColumn > 9 && !L2isShooting)
			L2playerSpriteColumn = 0;
		else if (L2playerSpriteColumn > 12){
			L2playerSpriteColumn = 0;
			L2isShooting = false;
		}
		L2playerSpriteColumn++;
		L2counter = 0;
	}
	L2counter += (1 * L2time);

}

void LevelTwoActivity::drawTiles()
{
	int tile;
	for (int y = 0; y < MAP_SIZEY; y++)
	{
		for (int x = 0; x < MAP_SIZEX; x++)
		{
			tile = atoi(L2map[y][x].c_str());


			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			glBindTexture(GL_TEXTURE_2D, L2tileTextures[tile]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glEnable(GL_TEXTURE_2D);
			glBegin(GL_POLYGON);

			glTexCoord2f(0, 1);
			glVertex3f(2 * float(x), 2 * float(y + 1), 0.0f);

			glTexCoord2f(0, 0);
			glVertex3f(2 * float(x), 2 * float(y), 0.0f);

			glTexCoord2f(1, 0);
			glVertex3f(2 * float(x + 1), 2 * float(y), 0.0f);

			glTexCoord2f(1, 1);
			glVertex3f(2 * float(x + 1), 2 * float(y + 1), 0.0f);

			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glPopMatrix();

		}
	}
}
GLuint LevelTwoActivity::loadTexture(string filename)
{

	GLuint texture = SOIL_load_OGL_texture(filename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

	return texture;
}
void LevelTwoActivity::readMapFile(string filename)
{
	using namespace std;

	ifstream in(filename);

	string line, field;


	vector<string> v;                // array of values for one line only

	while (getline(in, line))    // get next line in file
	{
		v.clear();
		stringstream ss(line);

		while (getline(ss, field, ','))  // break line into comma delimitted fields
		{
			v.push_back(field);  // add each field to the 1D array
		}

		L2map.push_back(v);  // add the 1D array to the 2D array
	}


}
void LevelTwoActivity::renderHUD()
{
	//rendering the hud
	glPushMatrix();

	gluOrtho2D(-400, 400, -300, 300);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	glPushMatrix();
	glTranslated(-20, 12, 0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2d(0, 0);
	glVertex2d(40, 0);
	glVertex2d(40, 8);
	glVertex2d(0, 8);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	//rendering the health bar of the player and png of text
	//Health Bar
	glPushMatrix();

	gluOrtho2D(-400, 400, -300, 300);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glPushMatrix();
	glTranslated(-13.7, 13, 0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2d(0, 0);
	glVertex2d(L2hero.playerHealth / 8, 0);
	glVertex2d(L2hero.playerHealth / 8, 1.2);
	glVertex2d(0, 1.2);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();
	glPopMatrix();

	//rendering badge in hud
	glPushMatrix();
	gluOrtho2D(-400, 400, -300, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(17.6, 12.5, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, badgeTexture);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);


	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(1, 0);
	glVertex2f(2, 0);
	glTexCoord2f(1, 1);
	glVertex2f(2, 2);
	glTexCoord2f(0, 1);
	glVertex2f(0, 2);


	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//text to screen
	glPushMatrix();
	gluOrtho2D(-400, 400, -300, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);

	// Position The Text On The Screen
	glRasterPos2f(-20, 13);

	glPrint("Health: ");	// Print GL Text To The Screen

	// Position The Text On The Screen
	glRasterPos2f(14.6, 13);

	glPrint(std::to_string((long double)L2hero.badgeCount) + " x ");	// Print GL Text To The Screen

	glRasterPos2f(1.6, 13);

	glPrint("Score:" + std::to_string((long double)L2hero.playerScore));	// Print GL Text To The Screen

	glPopMatrix();


}
void LevelTwoActivity::addEnemy()
{
	L2timer += L2time;
	if (L2enemies.size()<4){
		if (L2timer > 6){
			int randomX = rand() % 2;
			int randomY = rand() % 2;
			EnemyRobot rob(L2enemySpawnPoints[randomX], L2enemySpawnPoints[randomY], L2map, enemyTextureID, 2);
			L2enemies.push_back(rob);
			L2timer = 0;
		}
	}
}
void LevelTwoActivity::collectableCollisionCheck()
{
	for (int i = 0; i < L2collectables.size(); i++)
	{
		if (L2hero.checkForCollision(L2collectables[i].boundingBox))
		{
			//heart
			if ((int)L2collectables[i].type == 0)
			{
				if (L2hero.playerHealth < 50)
					L2hero.playerHealth += 50;
				else
					L2hero.playerHealth = 100;

				PlaySound("Sounds/heartPickUp.wav", NULL, SND_ASYNC | SND_FILENAME);
			}//gun turbo 
			else if ((int)L2collectables[i].type == 1)
			{
				L2hero.gunTurboFlag = true;
				L2hero.playerScore += 50;
				PlaySound("Sounds/gunPickUp.wav", NULL, SND_ASYNC | SND_FILENAME);
			}
			else//if collectable is badge
			{
				L2hero.badgeCount++;
				L2hero.playerScore += 500;
				PlaySound("Sounds/badgePickUp.wav", NULL, SND_ASYNC | SND_FILENAME);
				if (L2hero.badgeCount == 4)
				{
					EnemyRobot L2robot1(90, 30, L2map, enemyTextureID, 1);
					EnemyRobot L2robot2(90, 30, L2map, enemyTextureID, 1);
					EnemyRobot L2robot3(90, 30, L2map, enemyTextureID, 1);
					EnemyRobot L2robot4(90, 30, L2map, enemyTextureID, 1);
					L2enemies.push_back(L2robot1);
					L2enemies.push_back(L2robot2);
					L2enemies.push_back(L2robot3);
					L2enemies.push_back(L2robot4);
				}
			}
			L2collectables.erase(L2collectables.begin() + i);
		}
	}
}

void LevelTwoActivity::enemyOnEnemyCollisionCheck()
{
	//looping through all the enemies updating their positons and checking for collisions
	for (int i = 0; i < L2enemies.size(); i++)
	{
		//#####   NEED TO INSERT CHECK iF THE ENEMY IS IN A GIVEN RADIOUS OF THE PLAYER FIRST TO IMPROVE EFFIENCY
		for (int j = i + 1; j < L2enemies.size(); j++)
		{
			if (L2enemies[i].checkForCollision(L2enemies[j].boundingBox))
				L2enemies[i].update(L2time, playerX, playerY);

		}
		if (L2enemies[i].checkForCollision(L2hero.boundingBox))
		{
			L2hero.setCollisionFlag(true);
		}
		else
		{
			L2hero.setCollisionFlag(false);
		}
		L2enemies[i].update(L2time, playerX, playerY);

	}
}
void LevelTwoActivity::bulletsCollisionCheck()
{
	//checking collisions between player bullets and enemies, tile and off screen
	for (int i = 0; i < L2hero.bullets.size(); i++)
	{
		if (L2hero.bullets[i].isCollidingWithTile(L2map) ||
			L2hero.bullets[i].isOutOFBounds(playerX, playerY))
		{
			L2hero.bullets.erase(L2hero.bullets.begin() + i);
			break;
		}
		L2hero.bullets[i].update(L2time);
		for (int j = 0; j < L2enemies.size(); j++)
		{
			if (L2hero.bullets[i].checkForCollision(L2enemies[j].boundingBox))
			{
				L2enemies.erase(L2enemies.begin() + j);
				L2hero.bullets.erase(L2hero.bullets.begin() + i);
				L2hero.playerScore += 200;
				break;
			}

		}

	}
	//checking the enemies bullets against the player
	for (int i = 0; i < L2enemies.size(); i++)
	{
		for (int j = 0; j < L2enemies[i].bullets.size(); j++)
		{
			if (L2hero.checkForCollision(L2enemies[i].bullets[j].boundingBox))
			{
				L2hero.playerHealth -= 15;
				if (L2hero.playerHealth <= 0)
				{
					// Space
					app->game->shutdown();
					app->setCurrentActivity(app->endScreen);
				}
				L2enemies[i].bullets.erase(L2enemies[i].bullets.begin() + j);

			}
			else if (L2enemies[i].bullets[j].isCollidingWithTile(L2map))
			{
				L2enemies[i].bullets.erase(L2enemies[i].bullets.begin() + j);

			}
		}
	}
}
GLvoid LevelTwoActivity::glPrint(string fmt)					// Custom GL "Print" Routine
{
	stringstream stream;
	stream << fmt << " ";

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);

	glCallLists(stream.str().size(), GL_UNSIGNED_BYTE, stream.str().c_str());
	glPopAttrib();
}
GLvoid LevelTwoActivity::BuildFont(GLvoid)
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping
	base = glGenLists(96);

	font = CreateFont(-40,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_NORMAL,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		"Arial");					// Font Name

	oldfont = (HFONT)SelectObject(hDc, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDc, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDc, oldfont);							// Selects The Font We Want
	DeleteObject(font);
}

void LevelTwoActivity::readScore()
{
	ifstream inputFile("scores.txt");
	int num;
	inputFile >> num;
	inputFile.close();
	L2hero.playerScore = num;
}

void LevelTwoActivity::writeScore()
{

	ofstream myfileOut;
	myfileOut.open("scores.txt");
	myfileOut << L2hero.playerScore;
	myfileOut.close();

}
