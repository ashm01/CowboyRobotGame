#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "Bullet.h"
#include "SOIL.h"
#include "Matrix.h"
#include "Math.h"
#include "OpenGLApplication.h"		
#include <iostream>
using namespace::std;

#include <iostream>

const double pi = 3.14159265359;
const double speed = 30;
double angle;

Bullet::Bullet()
{
	Bullet(0, 0, 0, 1);
}
Bullet::Bullet(double _x, double _y, int direction, int level)
{
	
	this->x = _x; 
	this->y = _y; 
	this->bDirection = direction;
	this->boundingBox = BoundingBox(y + 0.5, y - 0.5, x - 0.5, x + 0.5);
	this->level = level;
}

Bullet::~Bullet(){

}

void Bullet::update(double deltaT)
{
	angle = bDirection*pi/180;
    double dx = cos(angle);
	double dy = sin(angle);
	
	
	x-= dx*deltaT*speed;
	y+= dy*deltaT*speed;
	this->boundingBox.updatePosition (y + 0.1, y - 0.1, x - 0.1, x + 0.1);
}


void Bullet::draw(GLuint bullet, double size)
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, bullet);
	
	glTranslated(x, y, 0);
	
	
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);

	
		glTexCoord2f(0, 0);
		glVertex2f(-size, -size);
		glTexCoord2f(1, 0);
		glVertex2f(size, -size);
		glTexCoord2f(1, 1);
		glVertex2f(size, size);
		glTexCoord2f(0, 1);
		glVertex2f(-size, size);


	glEnd();

	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

bool Bullet::isOutOFBounds(double playerX, double playerY)
{
	if(x>playerX+40||x<playerX-40||y>playerY+40||y<playerY-40)
		return true;
	return false;
}
bool Bullet::checkForCollision(BoundingBox otherBox)
{
	if (!(boundingBox.left > otherBox.right || boundingBox.right < otherBox.left ||
		boundingBox.bottom > otherBox.top || boundingBox.top < otherBox.bottom))
	{
		return true;
	}
	return false;

}

bool Bullet::isCollidingWithTile(vector< vector<string> > &map)
{

	//checking wheter the bullets is valid
	if (x > 198 || x < 0 || y > 198 || y < 0)
		return true;
	//getting the value of the tile within the bitmap of my map
	int tileValue = atoi(map[ceil((y-2) / 2)][ceil(x / 2)].c_str());
	if (level == 1){

		//if the tile is less then 3 i.e. grass or path no collision
		if (tileValue < 3 || tileValue == 6){
			return false;
		}
		else
		{
			return  true;
		}
	}
	else 
	{
		if (tileValue == 4){
			return true;
		}
		else
		{
			return  false;
		}
	}

}









