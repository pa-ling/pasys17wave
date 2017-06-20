#ifndef _CORE_H
#define _CORE_H

#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define assertEqualsDouble(expected, actual, epsilon) (assert(fabs((expected) - (actual)) < epsilon));

void init(double cFactor, unsigned int tPoints, double shiftFactor);
void simulate(void);
void output(void);
double* getNewValues(void);
int getArraySize(void);
void terminate(void);

#endif
