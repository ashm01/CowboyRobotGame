#pragma once
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "SOIL.h"
#include "Player.h"
#include "OpenGLApplication.h"			// Needed to access member functions and variables from OpenGLApplication
#include "BoundingBox.h"
#include <iostream>
#include <vector>
using std::vector;
class EnemyRobot
{
public:
	EnemyRobot();
	EnemyRobot(double xPos, double yPos, vector< vector<string> > &map, GLuint &texture, int level);
	void update(double deltaT, double playerX, double playerY);
	void draw();
	int roundDirection(int numToRound);
	void shoot();
	bool isCollidingWithTile();
	bool checkForCollision(BoundingBox otherBox);
	bool canShoot();
	~EnemyRobot();
	BoundingBox boundingBox;
	vector <Bullet> bullets;
private:
	double x, y;
	double counter;
	double speed;
	GLuint texture;
	double enemyDirection;
	int currentLevel;
	
	
	
};

