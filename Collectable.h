#pragma once

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "SOIL.h" 
#include <iostream>
#include "BoundingBox.h"

class Collectable
{
public:
	enum Type{ HEART, GUN, BADGE };
	Collectable();
	Collectable(double x, double y, GLuint texture, Type type);
	~Collectable();
	void draw(double size);
	BoundingBox boundingBox;
	int getCollectableType();
	bool checkForCollision(BoundingBox otherBox);
	Type type;
	
private:
	double x, y;
	GLuint texture;
	
};

