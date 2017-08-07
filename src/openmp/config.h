/**
 * @file config.h
 * @author Patrick Fehling (Christian Schütt)
 * @date 7 Aug 2017
 * @brief File containing functionality to read the config file
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
  * Returns a value for the length of each time step in 
  * the wave equation.
  * 
  * @return C
  */
double getC(void);

/**
  * Returns array size which is the length of the wave.
  *
  * @return array size
  */
unsigned int getDataSize(void);

/**
  * Returns the shift value which is the difference of
  * the inital waves.
  *
  * @return shift
  */
double getShift(void);

/**
  * Returns the Count of Steps which are executed before
  * the program exits.
  *
  * @return simulation steps
  */
unsigned int getSimulationSteps(void);

/**
  * Returns whether the gui shall be shown or not.
  *
  * @return 1 = show; 0 = do not show
  */
bool isGui(void);

/**
  * Reads the given config and program arguments and provides
  * those for other parts of the program.
  *
  * @param argc argument count of program
  * @param argv arguments of program
  * @return 0 if succesful, 1 if not
  */
int readConfig(int argc, char *argv[]);

#endif