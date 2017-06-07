/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */

#include "core.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TPOINTS 1000
#define PI 3.14159265358979323846

double	values[TPOINTS + 2],  /* values at time t */
	    oldval[TPOINTS + 2],  /* values at time (t-dt) */
	    newval[TPOINTS + 2];  /* values at time (t+dt) */

int     arrLen = sizeof(values) / sizeof(double);
double     c = 0.1;

void init()
{   
    values[0] = 0;
    oldval[0] = 0;
    oldval[arrLen-1] = 0;
    values[arrLen-1] = 0;
    for (int i = 1; i < arrLen-1; i++)
    {
        values[i] = sin((i+5)*0.02);
        oldval[i] = sin(i*0.02);
    }
}

void simulate()
{
	//#pragma omp parallel for
    for (int i = 1; i < arrLen-1; i++)
    { 
        newval[i] = (2* values[i]) - oldval[i] + c * (values[i-1] - (2 * values[i]) + values[i+1]);
    }
	
	//#pragma omp parallel for
    for (int i = 1; i < arrLen-1; i++)
    { 
        oldval[i] = values[i];
        values[i] = newval[i];
    }
}

void output()
{
    for (int i = 1; i < arrLen; i++)
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

