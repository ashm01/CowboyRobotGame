#include "Collectable.h"

double spin = 0;

Collectable::Collectable()
{
	Collectable(0, 0, texture, Type::HEART);
}
Collectable::Collectable(double x, double y, GLuint texture, Type dd)
{
	this->x = x;
	this->y = y;
	this->texture = texture;
	this->boundingBox = BoundingBox(y + 1, y - 1, x - 1, x + 1);
	this->type = dd;
	
}

void Collectable::draw(double size)
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTranslated(x, y, 0);
	glRotatef(spin, 0.0, 0.0, 1.0);

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

	spin += 0.5f;
	if (spin>360)
		spin = 0;


}

Collectable::~Collectable()
{
}

bool Collectable::checkForCollision(BoundingBox otherBox)
{
	if (!(boundingBox.left > otherBox.right || boundingBox.right < otherBox.left ||
		boundingBox.bottom > otherBox.top || boundingBox.top < otherBox.bottom))
	{
		return true;
	}
	return false;

}
int Collectable::getCollectableType()
{
	return (int)this->type;
}