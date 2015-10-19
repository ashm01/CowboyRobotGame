#include "Matrix.h"


void setIdentityMatrix(float *m)
{
	for (int i = 0; i < 9; i++)
	{
		m[i] = 0.0;
	}
	m[0] = 1.0;
	m[4] = 1.0;
	m[8] = 1.0;
}

void setRotationMatrix(float *m, float angle, int axis)
{
	setIdentityMatrix(m);
	switch (axis){
	case 0: 
		break;
	case 1: 
		break;
	case 2: 
		m[0] = m[4] = cos(angle);
		m[3] = -sin(angle);
		m[1] = -m[3];
		break;
	}
}

void setTranslationMatrix(float *m, float x, float y, float z)
{
	setIdentityMatrix(m);
	m[6] = x;
	m[7] = y;
	m[8] = z;
}

void MultiplyMatrix(float *matrix1, float *matrix2, float *resultMatrix)
{
	for (int i = 0; i < 3; i++){

		for (int j = 0; j < 3; j++){

			resultMatrix[j * 3 + i] = matrix1[i] * matrix2[3 * j] + matrix1[i + 3] * matrix2[3 * j + 1] + matrix1[i + 6] * matrix2[3 * j + 2];
		}
	}
}

void MultiplyMattixPostVector(float *m1, float *v1, float *v)
{
	for (int j = 0; j < 3; j++){

		v[j] = m1[j * 3] * v1[j] + m1[j * 3 + 1] * v1[j + 3] + m1[j * 3 + 2] * v1[j + 6];
	}
}