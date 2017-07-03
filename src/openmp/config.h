#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

double getC(void);
unsigned int getDataSize(void);
double getShift(void);
unsigned int getSimulationSteps(void);
bool isGui(void);
int readConfig(int argc, char *argv[]);

#endif