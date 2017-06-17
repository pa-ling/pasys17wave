#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"

#define MAXLINE 100

double c;
int arraySize, simulationSteps;
bool showGui;

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
        //printf("%s=%s;\n", configKey, configValue);
    }
    printf("%lf, %d, %d, %d \n", c, arraySize, showGui, simulationSteps);

    return 0;
}