/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */

#include "core.h"

#define TPOINTS 1101

double  *values,
        *oldval, 
        *newval;

int     arrLen; 
double  c = 0.1;

void init(double cFactor, unsigned int tPoints)
{   
    arrLen = tPoints;
    values = malloc(tPoints * sizeof(double));  
    oldval = malloc(tPoints * sizeof(double));
    newval = malloc(tPoints * sizeof(double));

    memset(values, 0, tPoints);
    memset(oldval, 0, tPoints);
    memset(newval, 0, tPoints);
    
    c = cFactor;

    for (int i = 1; i < arrLen-1; i++)
    {
       // values[i] = sin((i+1)*0.02);
        oldval[i] = sin(i*0.02);
        values[i] = sin(i*0.02);
    }
}

void simulate()
{
	#pragma omp parallel for
    for (int i = 1; i < arrLen-1; i++)
    { 
        newval[i] = (2 * values[i]) - oldval[i] + c * (values[i-1] - (2 * values[i]) + values[i+1]);
    }
	
	#pragma omp parallel for
    for (int i = 1; i < arrLen-1; i++)
    { 
        oldval[i] = values[i];
        values[i] = newval[i];
    }
}

void output()
{
    for (int i = 0; i < arrLen; i++)
    {
        printf("%f\n",values[i]);
    }
}

double* getNewValues()
{
    return newval;
}

int getArraySize()
{
    return TPOINTS;
}

void terminate()
{
    free(newval);
    free(oldval);
    free(values);
}

