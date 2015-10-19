//
// The structure of the Graphics 1 OpenGL template is explained in README.txt
//

/*
GameActivity implementation


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
#include "Boss.h"
#include "OpenGLApplication.h"			// Needed to access member functions and variables from OpenGLApplication
#include "GameActivity.h"
#include "Collectable.h"
#include <iostream>
#include <vector>
#include<sstream>
#include<fstream>
#include <cstdlib>     
#include <ctime>   

using std::vector;
std::string str;
vector <Collectable> collectables;
vector <EnemyRobot> enemies;
using namespace::std;
#define VIEW_SIZE 30.0	
#define MAP_SIZEX 100
#define MAP_SIZEY 100
GLuint texture[10];
vector< vector<string> > map;  // the 2D array
double enemySpawnPoints[] = {170,30};
bool boss_flag = false;
Boss boss;
double CAMERA_MOVEMENT_SPEED = 10.0;
double PLAYER_MOVEMENT_SPEED = 10.0;
int playerSpriteRow =2;
int playerSpriteColumn =1;
Player hero(196,141);
double counter;

double playerDirection, tempY, tempX = 0;

double time2, timer;
double limit = 0.1;
bool isShooting = false;
bool collision_flag = false;
int playerTileX, playerTileY = 0;




GameActivity::GameActivity(OpenGLApplication *app)
	: Activity(app)		// Call super constructora
{
	// Initialse camera and player positions to be a the origin
	camX = 180;
	camY = 139;
	playerX = 196;
	playerY = 141;
	
	
}    



/*
*
* ACTIVITY METHODS
* Put your application/game code here
*
*/
void GameActivity::initialise()
{
	Player hero(196, 141);
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
	bossTexture = loadTexture("Images/boss.png");
	texture[1] = loadTexture("Images/grassTile.png");
	texture[2] = loadTexture("Images/pathTile.png");
	texture[3] = loadTexture("Images/treeTile.png");
	texture[4] = loadTexture("Images/pathFenceLowerTile.png");
	texture[5] = loadTexture("Images/pathFenceHigherTile.png");
	texture[6] = loadTexture("Images/grass2Tile.png");
	texture[7] = loadTexture("Images/finishTile.png");
	readMapFile("MAP.txt");
	
	//initialisng collectables
	srand(time(NULL));
	
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, heartTexture, Collectable::Type::HEART));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, heartTexture, Collectable::Type::HEART));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, heartTexture, Collectable::Type::HEART));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, heartTexture, Collectable::Type::HEART));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, gunTexture, Collectable::Type::GUN));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, badgeTexture, Collectable::Type::BADGE));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, badgeTexture, Collectable::Type::BADGE));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, badgeTexture, Collectable::Type::BADGE));
	collectables.push_back(Collectable(rand() % (170 - 30) + 30, rand() % (170 - 30) + 30, badgeTexture, Collectable::Type::BADGE));
	AllocConsole();
		freopen("conout$", "w", stdout);
	BuildFont();
}


void GameActivity::shutdown()
{
	// Shutdown the activity; called at application finish

	// Delete the texture
	glDeleteTextures(1, &playerTextureID);
}



void GameActivity::onSwitchIn()
{
	// Activity switch in

	hero.playerHealth = 100;
	hero.playerScore = 0;
	// EXAMPLE CODE
	glClearColor(0.0,0.0,0.0,0.0);						//sets the clear colour to black
	//PlaySound("Sounds/air_raid.wav", NULL, SND_ASYNC | SND_FILENAME);

}

void GameActivity::onReshape(int width, int height)
{
	// If you need to do anything when the screen is resized, do it here

	// EXAMPLE CODE
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	double aspect = app->getAspectRatio();
	// The height of the visible area is defined by VIEW_SIZE. Split it half each way around the origin, hence the *0.5
	// Take the aspect ratio into consideration when computing the width of the visible area
	gluOrtho2D(-VIEW_SIZE*0.5*aspect, VIEW_SIZE*0.5*aspect,  -VIEW_SIZE*0.5, VIEW_SIZE*0.5);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void GameActivity::update(double deltaT, double prevDeltaT)
{
	
	time2 = deltaT;
	if (hero.playerHealth <= 0)
	{
		//player dies and game is ended
		app->game->shutdown();
		app->setCurrentActivity(app->endScreen);
	}
	addEnemy();
	
	tempX = playerX;
	tempY = playerY;
	//checking collision with heart
	//only checks when collectables are on map
	collectableCollisionCheck();
	enemyOnEnemyCollisionCheck();
	bulletsCollisionCheck();
	hero.update(playerX, playerY, playerSpriteRow, playerSpriteColumn, playerDirection);
	
	if(inputState->isKeyPressed(VK_LEFT))
	{
		if (camX> 20 && playerX<180)
			camX -= CAMERA_MOVEMENT_SPEED * deltaT;
	}
	if(inputState->isKeyPressed(VK_RIGHT))
	{
		if (camX< 180 && playerX>20)
			camX += CAMERA_MOVEMENT_SPEED * deltaT;
	}
	if(inputState->isKeyPressed(VK_UP))
	{
		if (camY< 185 && playerY>15)
			camY += CAMERA_MOVEMENT_SPEED * deltaT;
	}
	if(inputState->isKeyPressed(VK_DOWN))
	{
		if (camY> 15 && playerY<185)
			camY -= CAMERA_MOVEMENT_SPEED * deltaT;
	}

	// WASD control player
	//FIRST CHECKS TO SEE IF THE USER IS PRESSING DOWN TWO CONTROLS E.G. WS or AD etc

	if (inputState->isKeyPressed('S') && inputState->isKeyPressed('A'))
	{
		//s
		if (playerY > 2)
			playerY -= PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY> 15 && playerY<185&& !collision_flag)
			camY -= CAMERA_MOVEMENT_SPEED * deltaT;
		//a
		if (playerX > -1.5){
			playerX -= PLAYER_MOVEMENT_SPEED * deltaT;
		}
		if (camX > 20 && playerX < 180 && !collision_flag)
			camX -= CAMERA_MOVEMENT_SPEED * deltaT;
		
		changePlayerSprite(1, playerSpriteColumn);
		playerDirection = -45;


	}
	else if (inputState->isKeyPressed('S') && inputState->isKeyPressed('D'))
	{
		//s
		if (playerY > 2)
			playerY -= PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY> 15 && playerY<185 && !collision_flag)
			camY -= CAMERA_MOVEMENT_SPEED * deltaT;
		//d
		if (playerX < 199)
			playerX += PLAYER_MOVEMENT_SPEED * deltaT;
		if (camX< 180 && playerX>20 && !collision_flag)
			camX += CAMERA_MOVEMENT_SPEED * deltaT;
		changePlayerSprite(7, playerSpriteColumn);
		playerDirection = -135;
	}
	else if (inputState->isKeyPressed('W') == 1 && inputState->isKeyPressed('D') == 1)
	{
		//w
		if (playerY < 198)
			playerY += PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY< 185 && playerY>15 && !collision_flag)
			camY += CAMERA_MOVEMENT_SPEED * deltaT;
		//d
		if (playerX < 199)
			playerX += PLAYER_MOVEMENT_SPEED * deltaT;
		if (camX< 180 && playerX>20 && !collision_flag)
			camX += CAMERA_MOVEMENT_SPEED * deltaT;
		changePlayerSprite(5, playerSpriteColumn);
		playerDirection = 135;



	}
	else if (inputState->isKeyPressed('W') == 1 && inputState->isKeyPressed('A') == 1)
	{
		//w
		if (playerY < 198)
			playerY += PLAYER_MOVEMENT_SPEED * deltaT;
		if (camY< 185 && playerY>15 && !collision_flag)
			camY += CAMERA_MOVEMENT_SPEED * deltaT;
		//a
		if (playerX > 1.5){
			playerX -= PLAYER_MOVEMENT_SPEED * deltaT;
		}
		if (camX > 20 && playerX < 180 && !collision_flag)
			camX -= CAMERA_MOVEMENT_SPEED * deltaT;
		changePlayerSprite(3, playerSpriteColumn);
		playerDirection = 45;

	}
	else{

	//NOW CHECKS FOR THE SINGLE USE OF WSAD CONTROLS
		if (inputState->isKeyPressed('A'))
		{
			if (playerX > 1.5){
				playerX -= PLAYER_MOVEMENT_SPEED * deltaT;
			}
			if (camX > 20 && playerX < 180 && !collision_flag)
				camX -= CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(2, playerSpriteColumn);
			playerDirection = 0;
		}
		if (inputState->isKeyPressed('D'))
		{
			if (playerX < 199)
				playerX += PLAYER_MOVEMENT_SPEED * deltaT;
			if (camX< 179.5 && playerX>20 && !collision_flag)
				camX += CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(6, playerSpriteColumn);
			playerDirection = 180;
		}
		
		if (inputState->isKeyPressed('W'))
			{
			if (playerY < 198)
				playerY += PLAYER_MOVEMENT_SPEED * deltaT;
			if (camY< 184.5 && playerY>15 && !collision_flag)
				camY += CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(4, playerSpriteColumn);
			playerDirection = 90;
		}
		
		if (inputState->isKeyPressed('S'))
		{
			if (playerY > 2)
				playerY -= PLAYER_MOVEMENT_SPEED * deltaT;
			if (camY >15 && playerY < 185 && !collision_flag)
				camY -= CAMERA_MOVEMENT_SPEED * deltaT;
			changePlayerSprite(0,playerSpriteColumn);
			playerDirection = -90;
		}
		
		

	}
	//###COLLISON DETECTION ON TILES####
	//determing the player offset for tile collison detetction according to the direction
	//i.e checking what tile is determined bythe direction the hero is facing
	
	if (playerDirection == 0)
	{
		playerTileY = ceil((playerY - 3) / 2);//the game space is 200*200 however each tile is 2*2
		playerTileX = ceil((playerX - 2.5) / 2);// so dividing by 2 gives me the correect tile in the bitmap
	}

	else if (playerDirection == -135 || playerDirection == -90){
		playerTileX = ceil((playerX - 1.15) / 2);
		playerTileY = ceil((playerY - 4) / 2);
	}
	else if (playerDirection == 45)
	{
		playerTileX = ceil((playerX - 2) / 2);
		playerTileY = ceil((playerY - 4) / 2);
	}
	else if ( playerDirection == -45)
	{
		playerTileX = ceil((playerX - 3) / 2);
		playerTileY = ceil((playerY - 4) / 2);
	}
	else
	{
		playerTileX = ceil((playerX - 1.15) / 2);
		playerTileY = ceil((playerY - 3) / 2);
	}
	
	//had an error where the x tile would be over the range, these check fixed that
	if (playerTileY>99)
		playerTileY = 99;
	if (playerTileY<0)
		playerTileY = 0;
	if (playerTileX>99)
		playerTileX = 99;
	if (playerTileX<0)
		playerTileX = 0;
	//getting the value of the tile within the bitmap of my map
	int tileValue = atoi(map[playerTileY][playerTileX].c_str());
	//Checking whether play has made it to the finish area with all the badges
	//#####WHERE LEVEL 2 MAY BE STARTED
	if (tileValue == 7 && hero.badgeCount == 4)
	{
		//start level two
		writeScore();
		app->setCurrentActivity(app->levelTwo);
	}
	//if the tile is less then 3 i.e. grass or path no collision
	bool inGrassFlag;

	//slows the player down if he is in the long grass(tile 6)
	if (tileValue == 6)
	{
		collision_flag = false;
		inGrassFlag = true;
		
	}
	else if (tileValue < 3 || tileValue == 7){
		collision_flag = false;
		inGrassFlag = false;
	}
	else
	{
		inGrassFlag = false;
		collision_flag = true;
		playerX = tempX;
		playerY = tempY;
	}
	//spacebar press
	if (inputState->isKeyPressed(32))
	{
		playerSpriteColumn = 11;
		
	}
	//using shift to speed charecter up
	if (inputState->isKeyPressed(VK_SHIFT)&& hero.getCollisionFlag()==false && inGrassFlag == false)
	{
		CAMERA_MOVEMENT_SPEED = 25.0;
		PLAYER_MOVEMENT_SPEED = 25.0;
		limit = 0.05;
	}
	else if (hero.getCollisionFlag() == true)
	{
		CAMERA_MOVEMENT_SPEED = 3.0;
		PLAYER_MOVEMENT_SPEED = 3.0;
	}
	else if (inGrassFlag==true)
	{
		PLAYER_MOVEMENT_SPEED = 6;
		CAMERA_MOVEMENT_SPEED = 6;
	}
	else
	{ 
		CAMERA_MOVEMENT_SPEED = 10.0;
		PLAYER_MOVEMENT_SPEED = 10.0;
		limit = 0.1;
	}
	

}

void GameActivity::render()
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();									
	// Use the negated camera position as a translation; effectively we move the world and the camera so that the camera is at 0,0,0
	glTranslated(-camX, -camY, 0.0);

	// Render the  background
	
		drawTiles();
	
	
	//hero.boundingBox.draw();
	hero.draw(playerTextureID);
	
	//drawing Collectables
	for each (Collectable  c in collectables)
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
	for (int i = 0; i < hero.bullets.size(); i++)
	{
		//hero.bullets[i].boundingBox.draw();
		hero.bullets[i].draw(bullet1ID,0.1);
	}
	//drawing each enemy in the vector
	for (int i = 0; i < enemies.size(); i++)
	{
		//enemies[i].boundingBox.draw();
		enemies[i].draw();
	}
	if (boss_flag==true)
	{
		
		boss.draw();
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = 0; j < enemies[i].bullets.size(); j++)
		{
			//enemies[i].bullets[j].boundingBox.draw();
			enemies[i].bullets[j].draw(bullet2ID, 0.3);
		}
	}
	renderHUD();
	glFlush();

}



void GameActivity::onMouseDown(int button, int mouseX, int mouseY)
{
	// This method will be invoked when a mouse button is pressed
	// button: 0=LEFT, 1=MIDDLE, 2=RIGHT
	// mouseX and mouseY: position
}

void GameActivity::onMouseUp(int button, int mouseX, int mouseY)
{
	hero.shoot(1);
	isShooting = true;
	playerSpriteColumn = 11;
	//PlaySound("Sounds/shoot.wav", NULL, SND_ASYNC | SND_FILENAME);
}

void GameActivity::onMouseMove(int mouseX, int mouseY)
{
	// This method will be invoked when the mouse is moved
	// mouseX and mouseY: position
}

void GameActivity::onKeyDown(int key)
{
	// This method will be invoked when a key is pressed
}

void GameActivity::onKeyUp(int key)										// Called when key released
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
		hero.shoot(1);
		isShooting = true;
		playerSpriteColumn = 11;
		PlaySound("Sounds/shoot.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
	if (key == VK_F12)
	{
		

	}
}

void GameActivity::changePlayerSprite(int row, int column)
{
	playerSpriteRow = row;
	playerSpriteColumn = column;
	if (counter > limit){
		if (playerSpriteColumn > 9 && !isShooting)
			playerSpriteColumn = 0;
		else if (playerSpriteColumn > 12){
			playerSpriteColumn = 0;
			isShooting=false;}
		playerSpriteColumn++;
		counter = 0;
	}
	counter+=( 1*time2);
	
}

void GameActivity::drawTiles()
{
	int tile;
	for (int y = 0; y < MAP_SIZEY; y++)
	{
		for (int x = 0; x < MAP_SIZEX; x++)
		{
			tile = atoi(map[y][x].c_str());


			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			glBindTexture(GL_TEXTURE_2D, texture[tile]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_POLYGON);

				glTexCoord2f(0, 1);
				glVertex3f(2*float(x), 2*float(y + 1), 0.0f);

				glTexCoord2f(0, 0);
				glVertex3f(2*float(x), 2*float(y), 0.0f);

				glTexCoord2f(1, 0);
				glVertex3f(2*float(x + 1), 2*float(y), 0.0f);

				glTexCoord2f(1, 1);
				glVertex3f(2*float(x + 1), 2*float(y + 1), 0.0f);

			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glPopMatrix(); 

		}
	}
}
GLuint GameActivity::loadTexture(string filename)
{
		
		GLuint texture = SOIL_load_OGL_texture(filename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

		return texture;
}
void GameActivity::readMapFile(string filename)
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

		map.push_back(v);  // add the 1D array to the 2D array
	}


}
void GameActivity::renderHUD()
{
	//rendering the hud
	glPushMatrix();
	
	gluOrtho2D(-400, 400, -300, 300);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	
	glPushMatrix();
	glTranslated(-20,12,0);
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
		glVertex2d(hero.playerHealth/8, 0);
		glVertex2d(hero.playerHealth/8, 1.2);
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

	glPrint(std::to_string((long double)hero.badgeCount) + " x ");	// Print GL Text To The Screen

	glRasterPos2f(1.6, 13);

	glPrint("Score:" + std::to_string((long double)hero.playerScore));	// Print GL Text To The Screen

	glPopMatrix();
	

}
void GameActivity::addEnemy()
{
	timer += time2;
	if (enemies.size()<4){
		if (timer > 6){
			int randomX = rand() % 2;
			int randomY = rand() % 2;
			EnemyRobot rob(enemySpawnPoints[randomX], enemySpawnPoints[randomY], map, enemyTextureID,1);
			enemies.push_back(rob);
			timer = 0;
		}
	}
}
void GameActivity::collectableCollisionCheck()
{
	for (int i = 0; i < collectables.size(); i++)
	{
		if (hero.checkForCollision(collectables[i].boundingBox))
		{
			//heart
			if ((int)collectables[i].type==0)
			{
				if (hero.playerHealth < 50)
					hero.playerHealth += 50;
				else
					hero.playerHealth = 100;

				PlaySound("Sounds/heartPickUp.wav", NULL, SND_ASYNC | SND_FILENAME);

			}//gun turbo 
			else if ((int)collectables[i].type == 1)
			{
				hero.gunTurboFlag = true;
				hero.playerScore += 50;
				PlaySound("Sounds/gunPickUp.wav", NULL, SND_ASYNC | SND_FILENAME);
			}
			else//if collectable is badge
			{
				hero.badgeCount++;
				hero.playerScore += 500;
				PlaySound("Sounds/badgePickUp.wav", NULL, SND_ASYNC | SND_FILENAME);
				//adding the boss to the level when 4 badges have been found
				if (hero.badgeCount == 4)
				{
					//addBoss();
					EnemyRobot robot1(30, 40, map, enemyTextureID, 1);
					EnemyRobot robot2(30, 40, map, enemyTextureID, 1);
					EnemyRobot robot3(30, 40, map, enemyTextureID, 1);
					EnemyRobot robot4(30, 40, map, enemyTextureID, 1);
					enemies.push_back(robot1);
					enemies.push_back(robot2);
					enemies.push_back(robot3);
					enemies.push_back(robot4);
				}
			}
			collectables.erase(collectables.begin() + i);
		}
	}
}

void GameActivity::enemyOnEnemyCollisionCheck()
{
	//looping through all the enemies updating their positons and checking for collisions
	for (int i = 0; i < enemies.size(); i++)
	{
		//#####   NEED TO INSERT CHECK iF THE ENEMY IS IN A GIVEN RADIOUS OF THE PLAYER FIRST TO IMPROVE EFFIENCY
		for (int j = i + 1; j < enemies.size(); j++)
		{
			if (enemies[i].checkForCollision(enemies[j].boundingBox))
				enemies[i].update(time2, playerX, playerY);
			if (boss_flag == true)
			{
				if (boss.checkForCollision(enemies[i].boundingBox))
					boss.update(time2, playerX, playerY);
			}

		}
		if (enemies[i].checkForCollision(hero.boundingBox) || boss.checkForCollision(hero.boundingBox))
		{
			hero.setCollisionFlag(true);
		}
		else
		{
			hero.setCollisionFlag(false);
		}
		enemies[i].update(time2, playerX, playerY);

	}
}
void GameActivity::bulletsCollisionCheck()
{
	//checking collisions between player bullets and enemies, tile and off screen
	for (int i = 0; i < hero.bullets.size(); i++)
	{
		if (hero.bullets[i].isCollidingWithTile(map) ||
			hero.bullets[i].isOutOFBounds(playerX, playerY))
		{
			hero.bullets.erase(hero.bullets.begin() + i);
			break;
		}
		hero.bullets[i].update(time2);
		for (int j = 0; j < enemies.size(); j++)
		{
			if (hero.bullets[i].checkForCollision(enemies[j].boundingBox))
			{
				enemies.erase(enemies.begin() + j);
				hero.bullets.erase(hero.bullets.begin() + i);
				hero.playerScore += 200;
				break;
			}
			if (hero.bullets[i].checkForCollision(boss.boundingBox))
			{
				boss.bossHealth -= 25;
				hero.bullets.erase(hero.bullets.begin() + i);
				hero.playerScore += 50;
				break;
			}
			
		}
		
	}
	//checking the enemies bullets against the player
	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = 0; j < enemies[i].bullets.size(); j++)
		{
			if (hero.checkForCollision(enemies[i].bullets[j].boundingBox))
			{
				hero.playerHealth -= 15;
				enemies[i].bullets.erase(enemies[i].bullets.begin() + j);

			}
			else if (enemies[i].bullets[j].isCollidingWithTile(map))
			{
				enemies[i].bullets.erase(enemies[i].bullets.begin() + j);

			}
		}
	}
}
GLvoid GameActivity::glPrint(string fmt)					// Custom GL "Print" Routine
{
	stringstream stream;
	stream << fmt << " ";

	glPushAttrib(GL_LIST_BIT);							
	glListBase(base - 32);								
	
	glCallLists(stream.str().size(), GL_UNSIGNED_BYTE, stream.str().c_str());	
	glPopAttrib();										
}
GLvoid GameActivity::BuildFont(GLvoid)
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

void GameActivity::addBoss()
{
	
		Boss boss(35, 35, map, bossTexture, 1);
		PlaySound("Sounds/boss-sound.wav", NULL, SND_ASYNC | SND_FILENAME);
		boss_flag = true;
	
}

void GameActivity::readScore()
{
	ifstream in("scores.txt");
	in >> hero.playerScore;
	in.close();
}

void GameActivity::writeScore()
{
	
	ofstream myfile;
	myfile.open("scores.txt");
	myfile << hero.playerScore;
	myfile.close();

}