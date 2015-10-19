
#ifndef POLYOBJECTS_H
#define POLYOBJECTS_H

class point
{
public:
	float x;
	float y;
};

class bsphere
{
public:
	float radius;
	point centre;
};


class polygon
{
public:
	polygon();
	point *vertArray1;
	point *vertArray2;
	bsphere psphere;
};

#endif