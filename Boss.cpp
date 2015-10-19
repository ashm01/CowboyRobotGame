#include "Boss.h"
#include <vector>
using std::vector;


double tPassed, bossDirX, bossDirY, bossSpriteRow, bossSpriteColumn;
double bTileX, bTileY, bossRandX, bossRandY = 0;
double bossShootTimer = 0;
bool boss_collision_flag = false;
int bossSpriteLength = 4;
double timer2b = 2;
vector< vector<string> > levelMap;

Boss::Boss()
{


}

Boss::Boss(double xPos, double yPos, vector< vector<string> > &map, GLuint &texture, int level)
{
	this->x = xPos;
	this->y = yPos;
	this->counter = 0;
	levelMap = map;
	this->boundingBox = BoundingBox(yPos + 1,
		yPos - 1, xPos - 1, xPos + 1);
	this->enemyDirection = 0;
	this->speed = 8.0;
	this->texture = texture;
	this->currentLevel = level;
	this->bossHealth = 100;

}

void Boss::update(double deltaT, double playerX, double playerY)
{

	if (bossShootTimer > 1){
		shoot();
		bossShootTimer = 0;
	}
	bossShootTimer += (1 * deltaT);

	tPassed = deltaT;
	if (this->x)
		bossDirX = playerX - x;
	bossDirY = playerY - y;
	double distance = sqrt(bossDirX*bossDirX + bossDirY*bossDirY);//distance between 2 points
	//determing the update if the enemy is in the given range of the player
	if (distance < 30){
		//determing the direction of the enemy
		this->enemyDirection = atan2(bossDirY, bossDirX);
		this->enemyDirection = enemyDirection * (180 / 3.14159265359);//convert to degrees
		this->enemyDirection = roundDirection(enemyDirection);//rounds to the nearest 45 degress


		//calculating enemy movement
		bossDirX /= distance;
		bossDirY /= distance;

		//determing the reponse of the possible collisions
		if (isCollidingWithTile())
		{
			double colOffset = 1;
			switch ((int)enemyDirection)
			{
			case 180:
				x -= colOffset*speed*tPassed;
				break;
			case 135:
				x -= colOffset*speed*tPassed;
				y -= colOffset*speed*tPassed;
				break;
			case 90:
				y -= colOffset*speed*tPassed;
				break;
			case 45:
				x += colOffset*speed*tPassed;
				y -= colOffset*speed*tPassed;
				break;
			case 0:
				x += colOffset*speed*tPassed;
				break;
			case 360:
				x += colOffset*speed*tPassed;
				break;
			case 315:
				y += colOffset*speed*tPassed;
				x += colOffset*speed*tPassed;
			case 270:
				y += colOffset*speed*tPassed;
				break;
			case 225:
				y += colOffset*speed*tPassed;
				x -= colOffset*speed*tPassed;
			default:
				break;
			}
		}
		else if (boss_collision_flag)
		{
			//the response of colliding with objects based on direction
			double colOffset = 1;
			switch ((int)enemyDirection)
			{
			case 180:
				x -= colOffset*speed*tPassed;
				break;
			case 135:
				x -= colOffset*speed*tPassed;
				y -= colOffset*speed*tPassed;
				break;
			case 90:
				y -= colOffset*speed*tPassed;
				break;
			case 45:
				x += colOffset*speed*tPassed;
				y -= colOffset*speed*tPassed;
				break;
			case 0:
				x += colOffset*speed*tPassed;
				break;
			case 360:
				x += colOffset*speed*tPassed;
				break;
			case 315:
				y += colOffset*speed*tPassed;
				x += colOffset*speed*tPassed;
			case 270:
				y += colOffset*speed*tPassed;
				break;
			case 225:
				y += colOffset*speed*tPassed;
				x -= colOffset*speed*tPassed;
			default:
				break;
			}
		}
		else
		{
			x += bossDirX*speed*tPassed;
			y += bossDirY*speed*tPassed;
		}


	}
	else//moving the enemy randomly
	{
		////random movement goes here
		//	if (timer2b > 3){
		//		randY = ((rand() % 3 - 1) + 1) - 1;
		//		randX = ((rand() % 3 - 1) + 1) - 1;
		//		cout << randY << " " << randX << "   this robot is "<< this<< endl;
		//		timer2b = 0;
		//	}
		//	timer2b += (1 * deltaT);

		this->y += bossRandY*speed*tPassed;
		this->x += bossRandX*speed*tPassed;

	}

	this->boundingBox.updatePosition(y + 1.6, y - 1.2, x - 1.2, x + 1.2);
	//Deleting/updating enemy bullets
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].isOutOFBounds(playerX, playerY))
		{
			bullets.erase(bullets.begin() + i);
		}
		else
		{
			bullets[i].update(deltaT);
		}

	}

	cout << "X = " << this->x << "     Y = " << this->y << endl;
}

void Boss::draw()
{
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_TEXTURE_2D);

	GLfloat spriteWidth = 1 / 10.0, spriteHeight = 1 / 8.0;
	//Checking the direction of the enemy and changing the sprite row and column
	if (this->enemyDirection == 0)
	{
		bossSpriteRow = 1;
	}
	else if (this->enemyDirection == 90)
	{
		bossSpriteRow = 3;
	}
	else if (this->enemyDirection == 180)
	{
		bossSpriteRow = 5;
	}
	else if (this->enemyDirection == 270)
	{
		bossSpriteRow = 7;
	}
	else if (this->enemyDirection == 135)
	{
		bossSpriteRow = 4;
	}
	else if (this->enemyDirection == 45)
	{
		bossSpriteRow = 2;
	}
	else if (this->enemyDirection == 315)
	{
		bossSpriteRow = 0;
	}
	else if (this->enemyDirection == 225)
	{
		bossSpriteRow = 6;
	}
	//Changing the sprite based on time passed
	if (counter > 0.1){
		if (bossSpriteColumn > 7)
			bossSpriteColumn = -1;
		bossSpriteColumn++;
		counter = 0;
	}
	counter += (1 * tPassed);
	//drawing the enemy

	glBegin(GL_POLYGON);

		glTexCoord2f(bossSpriteColumn*spriteWidth, (bossSpriteRow + 1)*spriteHeight);
		glVertex2f(-bossSpriteLength, bossSpriteLength);
		glTexCoord2f(bossSpriteColumn*spriteWidth, bossSpriteRow * spriteHeight);
		glVertex2f(-bossSpriteLength, -bossSpriteLength);
		glTexCoord2f((bossSpriteColumn + 1) * spriteWidth, bossSpriteRow * spriteHeight);
		glVertex2f(bossSpriteLength, -bossSpriteLength);
		glTexCoord2f((bossSpriteColumn + 1)*spriteWidth, (bossSpriteRow + 1)*spriteHeight);
		glVertex2f(bossSpriteLength, bossSpriteLength);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();

}
bool Boss::isCollidingWithTile()
{

	//Determing whether emeny colldiing with map tiles
	if (this->enemyDirection == 180)
	{
		bTileY = ceil((y - 3) / 2);//the game space is 200*200 however each tile is 2*2
		bTileX = ceil((x - 2.5) / 2);// so dividing by 2 gives me the correect tile in the bitmap
	}

	else if (this->enemyDirection > 180){
		bTileX = ceil((x - 1) / 2);
		bTileY = ceil((y - 4) / 2);
	}
	else
	{
		bTileX = ceil((x - 0.5) / 2);
		bTileY = ceil((y - 2.2) / 2);
	}

	//getting the value of the tile within the bitmap of my map
	int tileValue = atoi(levelMap[bTileY][bTileX].c_str());
	if (this->currentLevel == 1)
	{

		//if the tile is less then 3 i.e. grass or path no collision
		if (tileValue < 3 || tileValue == 6){
			return false;
		}
		else
		{
			return  true;
		}
	}
	//level 2
	else
	{
		//if the tile is less then 3 i.e. grass or path no collision
		if (tileValue = 4){
			return false;
		}
		else
		{
			return  true;
		}
	}

}
int Boss::roundDirection(int numToRound)
{
	int multiple = 45;
	if (!(numToRound > -22 && numToRound < 23))
	{

		if (numToRound < 0) multiple = -multiple;
		int remainder = numToRound % multiple;

		numToRound = numToRound + multiple - remainder;
		//having to change the value to correspond the the players and bullets angle system
		if (numToRound == 0 || 180)
		{
			numToRound = abs(numToRound - 180);
		}
		return numToRound;
	}
	else return 180;
}
Boss::~Boss()
{
}
bool Boss::checkForCollision(BoundingBox otherBox)
{
	if (!(boundingBox.left > otherBox.right || boundingBox.right < otherBox.left ||
		boundingBox.bottom > otherBox.top || boundingBox.top < otherBox.bottom))
	{
		boss_collision_flag = true;
		return true;
	}
	boss_collision_flag = false;
	return false;

}

void Boss::shoot()
{
	bullets.push_back(Bullet(this->x, this->y, this->enemyDirection, this->currentLevel));
}

bool Boss::canShoot()
{
	return true;
}