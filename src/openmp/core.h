#ifndef _CORE_H
#define _CORE_H

#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define assertEqualsDouble(expected, actual, epsilon) (assert(fabs((expected) - (actual)) < epsilon));

void init(double cFactor);
void simulate(void);
void output(void);
double* getNewValues(void);
int getArraySize(void);

#endif
