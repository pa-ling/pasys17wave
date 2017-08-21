#include "core.h"

double *values,
    *oldval,
    *newval;

int taskid;
int numtasks;
int offset;
int leftOver;
int chunksize;
int updateSize;
MPI_Status status;

int dataSize, mode = 0;
double c = 0.1, shift;

void init(double cFactor, unsigned int tPoints, double shiftFactor)
{
    dataSize = tPoints;
    values = malloc(tPoints * sizeof(double));
    oldval = malloc(tPoints * sizeof(double));
    newval = malloc(tPoints * sizeof(double));

    memset(values, 0, tPoints);
    memset(oldval, 0, tPoints);
    memset(newval, 0, tPoints);

    c = cFactor;
    shift = shiftFactor;

    for (int i = 0; i < dataSize; i++)
    {
        oldval[i] = sin(i * 0.02);
        values[i] = sin((i + shift) * 0.02);
    }
}

void simulate(int leftEnd, int rightEnd)
{
    for (int i = leftEnd; i < rightEnd; i++)
    {
        newval[i] = (2 * values[i]) - oldval[i] + c * (values[i - 1] - (2 * values[i]) + values[i + 1]);
    }

    for (int i = leftEnd; i < rightEnd; i++)
    {
        oldval[i] = values[i];
        values[i] = newval[i];
    }
}

void output()
{
    for (int i = 0; i < dataSize; i++)
    {
        printf("%f\n", values[i]);
    }
}

double *getValue()
{
    return values;
}

int getArraySize()
{
    return dataSize;
}

void terminate()
{
    free(newval);
    free(oldval);
    free(values);
}

void scatterChunksToWorker(int numtasks)
{
    leftOver = dataSize % numtasks;
    chunksize = dataSize / numtasks;
    offset = chunksize + leftOver;
    updateSize = chunksize + 2;

    for (int dest = 1; dest < numtasks; dest++)
    {
        //the last worker node has less data to process because the end doesn't change at all
        if (dest + 1 == numtasks)
            updateSize--;

        MPI_Send(&offset, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Send(&chunksize, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Send(&updateSize, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Send(&oldval[offset - 1], updateSize, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        MPI_Send(&values[offset - 1], updateSize, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        offset += chunksize;
    }
    offset = 1;
    chunksize += leftOver;
}

void collectChunksFromWorkers(int numtasks)
{
    for (int source = 1; source < numtasks; source++)
    {
        MPI_Recv(&offset, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&oldval[offset], chunksize, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&values[offset], chunksize, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    }
}

void txUpdateToNeighbour(int taskid, int numtasks)
{
    // id of the predecessor thread (i.e. 3->2)
    int dest;
    int updatePosition;
    int sizeOfNeigbours = amountOfNeigbours(taskid, numtasks);
    int *neighbourIds = getNeigboursIds(taskid, numtasks);

    for (int i = 0; i < sizeOfNeigbours; i++)
    {
        // left boarder of the chunk
        dest = neighbourIds[i];
        updatePosition = offset;

        MPI_Send(&updatePosition, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Send(&oldval[updatePosition], 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        MPI_Send(&values[updatePosition], 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        // right boarder of the chunk
        updatePosition = offset + chunksize;
    }
    free(neighbourIds);
}

int amountOfNeigbours(int taskid, int numtasks)
{
    return (taskid > 0 && taskid < numtasks - 1) ? 2 : 1;
}

int *getNeigboursIds(int taskid, int numtasks)
{
    int *neighbourIds;
    int firstNode = 0;
    int lastNode = numtasks - 1;

    if (taskid == firstNode)
    {
        neighbourIds = (int *)malloc(1 * sizeof(int));
        neighbourIds[0] = firstNode + 1;
        return neighbourIds;
    }
    if (taskid == lastNode)
    {
        neighbourIds = (int *)malloc(1 * sizeof(int));
        neighbourIds[0] = lastNode - 1;
        return neighbourIds;
    }

    neighbourIds = (int *)malloc(2 * sizeof(int));
    neighbourIds[0] = taskid - 1;
    neighbourIds[1] = taskid + 1;
    return neighbourIds;
}

void rxUpdateFromNeighbour(int taskid, int numtasks)
{
    int source;
    int updatePosition;

    int sizeOfNeigbours = amountOfNeigbours(taskid, numtasks);
    int *neighbourIds = getNeigboursIds(taskid, numtasks);

    for (int i = 0; i < sizeOfNeigbours; i++)
    {
        // left boarder of the chunk
        source = neighbourIds[i];
        MPI_Recv(&updatePosition, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&oldval[updatePosition], 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&values[updatePosition], 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    }
    free(neighbourIds);
}

void updateNeighbours(int taskid, int numtasks)
{
    int parity = taskid % 2;
    // threads with even ids update their uneven neigbour(s)
    if (parity == 0)
    {
        txUpdateToNeighbour(taskid, numtasks);
        rxUpdateFromNeighbour(taskid, numtasks);
    }
    // vice versa
    else
    {
        rxUpdateFromNeighbour(taskid, numtasks);
        txUpdateToNeighbour(taskid, numtasks);
    }
}

void exeSimulationOnChunk()
{
    simulate(offset, offset + chunksize);
}

void rxChunkFromMaster()
{
    MPI_Recv(&offset, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&chunksize, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&updateSize, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&oldval[offset - 1], updateSize, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&values[offset - 1], updateSize, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, &status);
}

void txChunksToMaster()
{
    MPI_Send(&offset, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
    MPI_Send(&oldval[offset], chunksize, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);
    MPI_Send(&values[offset], chunksize, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);
}
