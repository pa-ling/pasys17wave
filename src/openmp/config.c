#include "config.h"

#define MAXLINE 100

double c, shift;
unsigned int arraySize, simulationSteps;
bool showGui;

double getC()
{
    return c;
}

unsigned int getDataSize()
 {
     return arraySize;
 }
 
double getShift()
{
    return shift;
}

unsigned int getSimulationSteps()
{
    return simulationSteps;
}

bool isGui()
{
    return showGui;
}

int readConfig()
{
    FILE * filePointer;
    char buffer[MAXLINE], configKey[20], configValue[10];
    int i, count;


    filePointer = fopen("parameters.conf","r");
    if(NULL == filePointer)
    {
        printf("error getting file");
        return 1;
    } 

    while(NULL != fgets(buffer, MAXLINE, filePointer))
    {
        count = 0;
        i = 0;

        if ('#' == buffer[0]) //ignore comments
        {
            continue;
        }
        
        while (' ' != buffer[i]) //get config key
        {
            configKey[i] = buffer[i];
            i++;
        }
        configKey[i++]='\0';
        count=i;

        while ('\n' != buffer[i]) //get config value
        {
            configValue[i-count] = buffer[i];
            i++;
        }
        configValue[i-count] = '\0';

        if (0 == strcmp(configKey, "C"))
        {
            c = atof(configValue); 
        } 
        else if (0 == strcmp(configKey, "ARRAY_SIZE"))
        {
            arraySize = atoi(configValue);
        } 
        else if (0 == strcmp(configKey, "SHOW_GUI"))
        {
            showGui = atoi(configValue);
        } 
        else if (0 == strcmp(configKey, "SIMULATION_STEPS"))
        {
            simulationSteps = atoi(configValue);
        }
        else if (0 == strcmp(configKey, "SHIFT"))
        {
            shift = atof(configValue);
        }
        //printf("%s=%s;\n", configKey, configValue);
    }
    printf("Wave Benchmark started with the following parameters:\n\tC: %lf\n\tShift: %lf\n\tArray size: %d\n\tGUI: %s\n\tSimulations steps: %d\n",
            c, shift, arraySize, showGui ? "true" : "false", simulationSteps);

    return 0;
}