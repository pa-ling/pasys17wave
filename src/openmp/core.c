/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */

#include "core.h"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

//added lib
#include <math.h>

#define TPOINTS 1000

double	values[TPOINTS + 2],  /* values at time t */
	    oldval[TPOINTS + 2],  /* values at time (t-dt) */
	    newval[TPOINTS + 2];  /* values at time (t+dt) */

int     i = 0;
int     startpoint = 1;
int     endpoint = 1000;
int     arrLen = sizeof(values) / sizeof(double);
int     c = 1;

void init(){

    for(int i = 0; i < arrLen; i++){
        values[i] = sin(i);
        oldval[i] = values[i]; 
    }
}

void simulate(){
    
	#pragma omp parallel for
    for (int i = startpoint; i <= endpoint; i++){ 
        newval[i] = (2* values[i]) - oldval[i] + c * (values[i-1] - (2 * values[i]) + values[i+1]);
    }
	
	#pragma omp parallel for
    for (int i = startpoint; i <= endpoint; i++){ 
        oldval[i] = values[i];
        values[i] = newval[i];
    }
}

void output(){
    printf("|\told\t|\tnew\t|\n\n"); 

    for (i = startpoint; i <= endpoint; i++){ 
        printf("%f\t|\t",oldval[i]); 
        printf("%f\n",values[i]);
    }
}

