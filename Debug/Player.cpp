#include "Player.h"
#include "SOIL.h"
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>	



int spriteRow = 2;
int spriteColumn = 1;
int length = 3;

Player::Player()
{
	Player(0, 0);
}

Player::Player(double xPos, double yPos)
{
	this->x = xPos;
	this->y = yPos;
	this->playerDirection = 100.00;
	this->collisionFlag = false;
	this->playerHealth = 100;
	this->playerScore = 0;
	this->badgeCount = 0;
}

void Player::update(double x, double y, int row, int column, int pDirection)
{
	this->x = x;
	this->y = y;
	spriteRow = row;
	spriteColumn = column;
	this->boundingBox.updatePosition(y + 2, y - 2, x - 1, x + 1);
	this->playerDirection = pDirection;

	
}


Player::~Player()
{
}


void Player::draw(GLuint player)
{
	glPushMatrix();

	glTranslated(x, y, 0.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, player);

	glEnable(GL_TEXTURE_2D);
	
	GLfloat spriteWidth = 1 / 14.0, spriteHeight = 1 / 10.0;

	glBegin(GL_POLYGON);

		glTexCoord2f(spriteColumn*spriteWidth, (spriteRow + 1)*spriteHeight);
		glVertex2f(-length, length);
		glTexCoord2f(spriteColumn*spriteWidth, spriteRow * spriteHeight);
		glVertex2f(-length, -length);
		glTexCoord2f((spriteColumn + 1) * spriteWidth, spriteRow * spriteHeight);
		glVertex2f(length, -length);
		glTexCoord2f((spriteColumn + 1)*spriteWidth, (spriteRow + 1)*spriteHeight);
		glVertex2f(length, length);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();


}
void Player::setBoundingBox(BoundingBox box)
{
	this->boundingBox = box;
}

bool Player::checkForCollision(BoundingBox otherBox)
{
	
	return !(boundingBox.left > otherBox.right || boundingBox.right < otherBox.left ||
		boundingBox.bottom > otherBox.top || boundingBox.top < otherBox.bottom);
}

void Player::shoot(int level)
{
	
	double xOff, yOff;//for the bullet position offset based on the players direction
						//switch to determine the offset for the bullet when its fired from the player
	switch ((int)playerDirection)
	{
	case 0://player looking left
		xOff = -2.8;
		yOff = 1.2;
		break;
	case 180://player looking right
		xOff = 2.8;
		yOff = 0.3;
		break;
	case 90://player looking up
		xOff = 0.8;
		yOff = 2.7;
		break;
	case -90:// player looking down
		xOff = -0.7;
		yOff = -1.3;
		break;
	case 45://player looking top left
		xOff = -1.4;
		yOff = 2.3;
		break;
	case 135://player looking top right
		xOff = 2.8;
		yOff = 1.4;
		break;
	case -45:// player looking bottom left
		xOff = -2.6;
		yOff = 0;
		break;
	case -135: // player looking bottom right
		xOff = 1.7;
		yOff = -0.8;
		break;
	}
	if (!gunTurboFlag)
	{
		this->bullets.push_back(Bullet(this->x + xOff, this->y + yOff, playerDirection, level));
	}
	else
	{
		this->bullets.push_back(Bullet(this->x + xOff, this->y + yOff, playerDirection, level));
		this->bullets.push_back(Bullet(this->x + xOff +0.5, this->y + yOff+0.5, playerDirection, level));
		this->bullets.push_back(Bullet(this->x + xOff -0.5, this->y + yOff-0.5, playerDirection, level));
	}
	
}
void Player::setCollisionFlag(bool answer)
{
	this->collisionFlag = answer;
}

bool Player::getCollisionFlag()
{
	return this->collisionFlag;
}


