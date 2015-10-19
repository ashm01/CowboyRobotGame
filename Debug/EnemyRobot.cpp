#include "EnemyRobot.h"
#include <vector>
using std::vector;


double timePassed, dirX, dirY, enemySpriteRow, enemySpriteColumn;
double eTileX, eTileY, currentX, currentY , randX, randY = 0;
double shootTimer = 0;
bool enemy_collision_flag = false;
int enSpriteLength = 2.5;
double timer2 = 2;
vector< vector<string> > worldMap;

EnemyRobot::EnemyRobot()
{
	
	
}

EnemyRobot::EnemyRobot(double xPos, double yPos, vector< vector<string> > &map, GLuint &texture, int level)
{
	this->x = xPos;
	this->y = yPos;
	this->counter = 0;
	worldMap = map;
	this->boundingBox = BoundingBox(yPos + 1, 
		yPos - 1, xPos - 1, xPos + 1);
	this->enemyDirection = 0;
	this->speed = 8.0;
	this->texture = texture;
	this->currentLevel = level;
	
}

void EnemyRobot::update(double deltaT, double playerX, double playerY)
{
	
	if (shootTimer > 1){
		shoot();
		shootTimer = 0;
	}
	shootTimer += (1 * deltaT);
	
	timePassed = deltaT;
	if (this->x )
	dirX = playerX - x;
	dirY = playerY - y;
	double distance = sqrt(dirX*dirX + dirY*dirY);//distance between 2 points
	//determing the update if the enemy is in the given range of the player
	if (distance < 30){
		//determing the direction of the enemy
		this->enemyDirection = atan2(dirY, dirX);
		this->enemyDirection = enemyDirection * (180 / 3.14159265359);//convert to degrees
		this->enemyDirection = roundDirection(enemyDirection);//rounds to the nearest 45 degress


		//calculating enemy movement
		dirX /= distance;
		dirY /= distance;

		//determing the reponse of the possible collisions
		if (isCollidingWithTile())
		{
			double colOffset = 1;
			switch ((int)enemyDirection)
			{
			case 180:
				x -= colOffset*speed*timePassed;
				break;
			case 135:
				x -= colOffset*speed*timePassed;
				y -= colOffset*speed*timePassed;
				break;
			case 90:
				y -= colOffset*speed*timePassed;
				break;
			case 45:
				x += colOffset*speed*timePassed;
				y -= colOffset*speed*timePassed;
				break;
			case 0:
				x += colOffset*speed*timePassed;
				break;
			case 360:
				x += colOffset*speed*timePassed;
				break;
			case 315:
				y += colOffset*speed*timePassed;
				x += colOffset*speed*timePassed;
			case 270:
				y += colOffset*speed*timePassed;
				break;
			case 225:
				y += colOffset*speed*timePassed;
				x -= colOffset*speed*timePassed;
			default:
				break;
			}
		}
		else if (enemy_collision_flag)
		{
			//the response of colliding with objects based on direction
			double colOffset = 1;
			switch ((int)enemyDirection)
			{
			case 180:
				x -= colOffset*speed*timePassed;
				break;
			case 135:
				x -= colOffset*speed*timePassed;
				y -= colOffset*speed*timePassed;
				break;
			case 90:
				y -= colOffset*speed*timePassed;
				break;
			case 45:
				x += colOffset*speed*timePassed;
				y -= colOffset*speed*timePassed;
				break;
			case 0:
				x += colOffset*speed*timePassed;
				break;
			case 360:
				x += colOffset*speed*timePassed;
				break;
			case 315:
				y += colOffset*speed*timePassed;
				x += colOffset*speed*timePassed;
			case 270:
				y += colOffset*speed*timePassed;
				break;
			case 225:
				y += colOffset*speed*timePassed;
				x -= colOffset*speed*timePassed;
			default:
				break;
			}
		}
		else
		{
			x += dirX*speed*timePassed;
			y += dirY*speed*timePassed;
		}

		
	}
	else//moving the enemy randomly
	{
		////random movement goes here
		//	if (timer2 > 3){
		//		randY = ((rand() % 3 - 1) + 1) - 1;
		//		randX = ((rand() % 3 - 1) + 1) - 1;
		//		cout << randY << " " << randX << "   this robot is "<< this<< endl;
		//		timer2 = 0;
		//	}
		//	timer2 += (1 * deltaT);
			
			this->y += randY*speed*timePassed;
			this->x += randX*speed*timePassed;
		
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
	
	
}

void EnemyRobot::draw()
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
		enemySpriteRow = 1;
	}
	else if (this->enemyDirection == 90)
	{
		enemySpriteRow = 3;
	}
	else if (this->enemyDirection == 180)
	{
		enemySpriteRow = 5;
	}
	else if (this->enemyDirection == 270)
	{
		enemySpriteRow = 7;
	}
	else if (this->enemyDirection == 135)
	{
		enemySpriteRow = 4;
	}
	else if (this->enemyDirection == 45)
	{
		enemySpriteRow = 2;
	}
	else if (this->enemyDirection == 315)
	{
		enemySpriteRow = 0;
	}
	else if (this->enemyDirection == 225)
	{
		enemySpriteRow = 6        ;
	}
	//Changing the sprite based on time passed
	if (counter > 0.1){
		if (enemySpriteColumn > 7)
			enemySpriteColumn = -1;
		enemySpriteColumn++;
		counter = 0;
	}
	counter += (1 * timePassed);
	//drawing the enemy
	
	glBegin(GL_POLYGON);

		glTexCoord2f(enemySpriteColumn*spriteWidth, (enemySpriteRow + 1)*spriteHeight);
		glVertex2f(-enSpriteLength, enSpriteLength);
		glTexCoord2f(enemySpriteColumn*spriteWidth, enemySpriteRow * spriteHeight);
		glVertex2f(-enSpriteLength, -enSpriteLength);
		glTexCoord2f((enemySpriteColumn + 1) * spriteWidth, enemySpriteRow * spriteHeight);
		glVertex2f(enSpriteLength, -enSpriteLength);
		glTexCoord2f((enemySpriteColumn + 1)*spriteWidth, (enemySpriteRow + 1)*spriteHeight);
		glVertex2f(enSpriteLength, enSpriteLength);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	
}
bool EnemyRobot::isCollidingWithTile()
{
	
	//Determing whether emeny colldiing with map tiles
	if (this->enemyDirection == 180)
	{
		eTileY = ceil((y - 3) / 2);//the game space is 200*200 however each tile is 2*2
		eTileX = ceil((x - 2.5) / 2);// so dividing by 2 gives me the correect tile in the bitmap
	}

	else if (this->enemyDirection > 180){
		eTileX = ceil((x - 1) / 2);
		eTileY = ceil((y - 4) / 2);
	}
	else
	{
		eTileX = ceil((x - 0.5) / 2);
		eTileY = ceil((y - 2.2) / 2);
	}
	//had an error where the x tile would be over the range, these check fixed that
	if (eTileY>99)
		eTileY = 99;
	if (eTileY<0)
		eTileY = 0;
	if (eTileX>99)
		eTileX = 99;
	if (eTileX<0)
		eTileX = 0;
	//getting the value of the tile within the bitmap of my map
	int tileValue = atoi(worldMap[eTileY][eTileX].c_str());
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
int EnemyRobot::roundDirection(int numToRound)
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
EnemyRobot::~EnemyRobot()
{
}
bool EnemyRobot::checkForCollision(BoundingBox otherBox)
{
	if(!(boundingBox.left > otherBox.right || boundingBox.right < otherBox.left ||
		boundingBox.bottom > otherBox.top || boundingBox.top < otherBox.bottom))
	{
		enemy_collision_flag = true;
		return true;
	}
	enemy_collision_flag = false;
	return false;
	
}

void EnemyRobot::shoot()
{
	bullets.push_back(Bullet(this->x, this->y, this->enemyDirection, this->currentLevel));
}

bool EnemyRobot::canShoot()
{
	return true;
}