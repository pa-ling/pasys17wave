#ifndef _CORE_H
#define _CORE_H

#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
  * Initializes the wave with the results of a sinus function.
  *
  * @param cFactor time step size
  * @param tPoints  array length
  * @param shiftFactor difference between initial waves
  */
void init(double cFactor, unsigned int tPoints, double shiftFactor);

/**
  * Executes on simulation step using the wave equation.
  */
void simulate(void);

/**
  * Outputs the current state of most recent values.
  */
void output(void);

/**
  * Returns the current state of most recent values.
  *
  * @return pointer to array of the newest values
  */
double* getNewValues(void);

/**
  * Return lengths of the value arrays
  *
  * @return lengths
  */
int getArraySize(void);

/**
  * Frees all allocated memory.
  */
void terminate(void);

#endif
