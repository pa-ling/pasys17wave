/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */

#include "core.h"

double  *values,
        *oldval, 
        *newval;

int     arrLen, mode = 0; 
double  c = 0.1, shift;

void init(double cFactor, unsigned int tPoints, double shiftFactor)
{   
    arrLen = tPoints;
    values = malloc(tPoints * sizeof(double));  
    oldval = malloc(tPoints * sizeof(double));
    newval = malloc(tPoints * sizeof(double));

    memset(values, 0, tPoints);
    memset(oldval, 0, tPoints);
    memset(newval, 0, tPoints);
    
    c = cFactor;
    shift = shiftFactor;

    for (int i = 1; i < arrLen-1; i++)
    {
        values[i] = sin((i+shift)*0.02);
        oldval[i] = sin(i*0.02);
    }
}

void simulate(int leftEnd, int rightEnd)
{
    
    for (int i = leftEnd + 1 ; i < rightEnd; i++)
    {
        newval[i] = (2 * values[i]) - oldval[i] + c * (values[i-1] - (2 * values[i]) + values[i+1]);
    }

    for(int i = leftEnd + 1; i < rightEnd; i++)
    {
        oldval[i] = values[i];
        values[i] = newval[i];
    }
}

void output()
{
    for (int i = 0; i < arrLen; i++)
    {
        printf("%f ",values[i]);
    }
    printf("\n");
}

double* getNewValues()
{
     return newval;
}

double* getValue()
{
    return values;
}

double* getOldVal()
{
    return oldval;
}


int getArraySize()
{
    return arrLen;
}

void terminate()
{
    free(newval);
    free(oldval);
    free(values);
}



