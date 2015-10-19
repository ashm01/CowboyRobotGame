#pragma once
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "SOIL.h"
#include "OpenGLApplication.h"
#include <iostream>
class BoundingBox
{
public:
	BoundingBox(double top, double bottom, double left, double right);
	BoundingBox();
	~BoundingBox();
	int is_point_in_rectangle(BoundingBox rect, int x, int y);
	void updatePosition(double top, double bottom, double left, double right);
	void draw();
	double top, bottom,left, right;
};

