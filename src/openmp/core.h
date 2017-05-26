#ifndef _CORE_H
#define _CORE_H
#include <assert.h>
#include <math.h>

#define assertEqualsDouble(expected, actual, epsilon) (assert(fabs((expected) - (actual)) < epsilon));

void init(void);
void simulate(void);
void output(void);

#endif
