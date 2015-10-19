

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "SOIL.h"
#include"BoundingBox.h"
#include "OpenGLApplication.h"			// Needed to access member functions and variables from OpenGLApplication
#include <string>
#include <iostream>
#include <vector>
using std::vector;
using std::string;

class Bullet{

	
public:
	Bullet();
	Bullet(double _x, double _y, int direction, int level);
	~Bullet();
	void update(double deltaT);
	void draw(GLuint background, double size);
	bool isOutOFBounds(double playerX, double playerY);
	bool checkForCollision(BoundingBox otherBox);
	bool Bullet::isCollidingWithTile(vector< vector<string> > &map);//need to do this!!!!
	BoundingBox boundingBox;
private:
	double x, y;
	double bDirection;
	int level;
};

