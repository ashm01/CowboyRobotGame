#pragma once

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "SOIL.h" 
#include <iostream>
#include "BoundingBox.h"
#include "Bullet.h"
using namespace::std;


class Player
{
public:
	Player();
	Player(double xPos, double yPos);
	~Player();//Destructor - Delete
	void update(double x, double y, int row, int column, int pDirection);
	void draw(GLuint player);
	void shoot(int level);
	void changeSprite(int row, int column, double time);
	void setBoundingBox(BoundingBox box);
	bool checkForCollision(BoundingBox otherBox);
	BoundingBox boundingBox;
	vector <Bullet> bullets;
	double playerHealth;
	int playerScore;
	int badgeCount;
	bool gunTurboFlag;
	void setCollisionFlag(bool answer);
	bool getCollisionFlag();
private:
	double x, y, playerDirection;
	bool collisionFlag;
	
	

};

