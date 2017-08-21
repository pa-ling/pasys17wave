/**
 * @file core.h
 * @author Christian Schütt (Patrick Fehling)
 * @date 7 Aug 2017
 * @brief File containing calculation logic
 */

#ifndef _CORE_H
#define _CORE_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define MASTER 0

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
  * @param startPoint the starting point to start the simulation 
  * @param endPoint the ending point to finish the simulation  
  */
void simulate(int startPoint, int endPoint);

/**
  * Outputs the current state of most recent values.
  */
void output(void);

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

/**
  * Returns the current state of current values.
  *
  * @return pointer to array of the current values
  */
double *getValue(void);

/**
  * Scatters the chunks of data to the worker nodes by the master node
  * @param numtasks number of tasks to spread the problem over 
  */
void scatterChunksToWorker(int numtasks);

/**
  * Collects the chunks of data by the master node.
  * @param numtasks number of tasks to spread the 
  */
void collectChunksFromWorkers(int numtasks);

/**
  * Receives the chunk of data from the master.  
  */
void rxChunkFromMaster(void);

/**
  * Transmits the assigned chunk of data to the master node.
  */
void txChunksToMaster(void);

/**
  * Works a wrapper funktion to execute the simulate(int startPoint, int endPoint).
  */
void exeSimulationOnChunk(void);

/**
  * Works as a wrapper function to update the possible right and left neighbour depending on the data chunk position.
  * That means the master node updates only its right neigbour. The most significant working node
  * updates only its left neighbour. All other working nodes in between update its right and left neigbour. 
  */
void updateNeighbours(int taskid, int numtasks);

/**
  * Transmits all values to neighbours which have to be updated. 
  * @param taskid taskid of the specific
  * @param numtask total number of operation nodes
  */
void txUpdateToNeighbour(int taskid, int numtasks);

/**
  * Receives all values from neighbours which have to be updated. 
  * @param taskid taskid of the specific 
  * @param numtask total number of operation nodes
  */
void rxUpdateFromNeighbour(int taskid, int numtasks);

/**
  * Returns a integer array with the node ids of the neighbours
  * @param taskid taskid of the specific 
  * @param numtask total number of operation nodes 
  * @return pointer to array of neighbour node ids
  */
int *getNeigboursIds(int taskid, int numtasks);

/**
  * Return a integer how many neighbours the specific node has
  * @param taskid taskid of the specific 
  * @param numtask total number of operation nodes
  * @return integer of number of neighbours
  */
int amountOfNeigbours(int taskid, int numtasks);
#endif
