#include "BoundingBox.h"
#include <iostream>

using namespace::std;
BoundingBox::BoundingBox(double top, double bottom, double left,double right)
{
	this->top = top;
	this->bottom = bottom;
	this->left = left;
	this->right = right;

}
void BoundingBox::updatePosition(double top, double bottom, double left, double right)
{
	this->top = top;
	this->bottom = bottom;
	this->left = left;
	this->right = right;
}
BoundingBox::BoundingBox()
{
	BoundingBox(0, 0, 0, 0);
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::draw()
{
	glPushMatrix();
	
	double x = (this->right + this->left) / 2;
	double y = (this->top + this->bottom) / 2;
	//cout << "left = " << this->left << " ¬¬ right = " << this->right << "¬¬ x = "<<x<<"¬¬ y = "<<y<< endl;
	glTranslated(x, y, 0);

	
	
	glBegin(GL_POLYGON);

		
		glTexCoord2f(0, 0);
		glVertex2f(-(x - this->left), -(y- this->bottom));
		glTexCoord2f(1, 0);
		glVertex2f(this->right - x, -(y - this->bottom));
		glTexCoord2f(1, 1);
		glVertex2f(this->right - x, this->top - y);
		glTexCoord2f(0, 1);
		glVertex2f(-(x- this->left), this->top - y);


	glEnd();
	glFlush();
	glPopMatrix();
}
