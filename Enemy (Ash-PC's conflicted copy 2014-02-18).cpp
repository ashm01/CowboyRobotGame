#include "Enemy.h"

double speed = 8.0;
double counter = 0;
double timePassed, dirX, dirY, enemySpriteRow, enemySpriteColumn;

Enemy::Enemy()
{
}

Enemy::Enemy(double xPos, double yPos)
{
	this->x = xPos;
	this->y = yPos;
}

void Enemy::update(double deltaT, double playerX, double playerY)
{
	timePassed = deltaT;
	dirX = playerX - x;
	dirY = playerY - y;
	double hyp = sqrt(dirX*dirX + dirY*dirY);
	dirX /= hyp;
	dirY /= hyp;
	x += dirX*speed*timePassed;
	y += dirY*speed*timePassed;
}

void Enemy::draw(GLuint enemy)
{
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, enemy);

	glEnable(GL_TEXTURE_2D);
	int vHeight = 2;
	GLfloat spriteWidth = 1 / 4.0, spriteHeight = 1 / 4.0;
	//Checking the direction of the enemy and changing the sprite row and column
	if (dirX < 0 && dirY > -0.3 && dirY < 0.3)
	{
		enemySpriteRow = 2;
	}
	else if (dirX >= 0 && dirY > -0.3 && dirY < 0.3){

		enemySpriteRow = 1;

	}
	else if (dirY < 0)
	{

		enemySpriteRow = 3;
	}
	else if (dirY >= 0)
	{

		enemySpriteRow = 0;
	}

	if (counter > 1){
		if (enemySpriteColumn > 2)
			enemySpriteColumn = -1;
		enemySpriteColumn++;
		counter = 0;
	}
	counter += (1 * timePassed);

	glBegin(GL_POLYGON);

	glTexCoord2f(enemySpriteColumn*spriteWidth, (enemySpriteRow + 1)*spriteHeight);
	glVertex2f(-vHeight, vHeight);
	glTexCoord2f(enemySpriteColumn*spriteWidth, enemySpriteRow * spriteHeight);
	glVertex2f(-vHeight, -vHeight);
	glTexCoord2f((enemySpriteColumn + 1) * spriteWidth, enemySpriteRow * spriteHeight);
	glVertex2f(vHeight, -vHeight);
	glTexCoord2f((enemySpriteColumn + 1)*spriteWidth, (enemySpriteRow + 1)*spriteHeight);
	glVertex2f(vHeight, vHeight);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	
}

Enemy::~Enemy()
{
}
