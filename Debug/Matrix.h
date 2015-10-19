#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <math.h>



void printMat(float *);
void setIdentityMatrix(float *);
void setRotationMatrix(float *, float, int);
void setTranslationMatrix(float *, float, float, float);
void MultiplyMatrix(float *,float *,float *);
void MultiplyMatrixPostVector(float *M, float *vo, float *v);

#endif