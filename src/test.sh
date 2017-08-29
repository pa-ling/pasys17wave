#!/bin/bash

#global variables
resultsFile="results.txt"
grepValue="real"
simulations=1000

function calculateAverage {
    sum=0
    for i in `seq 1 10`;
    do
        if [ "$5" = "OpenMP" ] 
        then
            timeOutput=$( { time -p OMP_NUM_THREADS=$1 $6 $2 $3 $4; } 2>&1 )
        elif [ "$5" = "OpenMPI" ] 
        then
            timeOutput=$( { time -p mpiexec -np $1 $6 $2 $3 $4; } 2>&1 )
        fi

        result=$(echo "$timeOutput" | grep $grepValue)
        resultTime=${result:5:4}
        resultTimes[$i]=$resultTime
        echo $resultTime
        sum=$(echo $sum + $resultTime | bc -l)
    done

    average=$(echo $sum / ${#resultTimes[@]} | bc -l)
    echo "$1 $3 $4: ${average:0:9}" >> $resultsFile
}

function breakLineToResultsFile {
    echo "----------------------" >> $resultsFile
    echo "$1 with $2 Threads" >> $resultsFile
    echo "----------------------" >> $resultsFile
}

#clean up 
echo "Benchmark:"> $resultsFile

#calculateAverage <THREAD_INVOLVED> <CONFIG_FILE> <ARRAY_SIZE> <STEP_SIZE> <LIBRARY> <PATH_TO_EXECUTABLE>

program_OpenMP="./openmp/wave"
configFile_OpenMP="./openmp/wave.conf"
UsedLibrary="OpenMP"

threads="1"
echo "OMP_NUM_THREADS=1"
breakLineToResultsFile $UsedLibrary $threads

breakLineToResultsFile $UsedLibrary $threads

calculateAverage $threads $configFile_OpenMP 0 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 500000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 1000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 5000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 10000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 15000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 20000000 $simulations $UsedLibrary $program_OpenMP


echo "OMP_NUM_THREADS=2"
threads="2"
breakLineToResultsFile $UsedLibrary $threads

calculateAverage $threads $configFile_OpenMP 0 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 500000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 1000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 5000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 10000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 15000000 $simulations $UsedLibrary $program_OpenMP
calculateAverage $threads $configFile_OpenMP 20000000 $simulations $UsedLibrary $program_OpenMP

program_OpenMPI="./openmpi/wave"
configFile_OpenMPI="./openmpi/wave.conf"
UsedLibrary="OpenMPI"

threads="1"
breakLineToResultsFile $UsedLibrary $threads

calculateAverage $threads $configFile_OpenMPI 0 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 500000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 1000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 5000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 10000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 15000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 20000000 $simulations $UsedLibrary $program_OpenMPI

threads="2"
breakLineToResultsFile $UsedLibrary $threads

calculateAverage $threads $configFile_OpenMPI 0 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 500000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 1000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 5000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 10000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 15000000 $simulations $UsedLibrary $program_OpenMPI
calculateAverage $threads $configFile_OpenMPI 20000000 $simulations $UsedLibrary $program_OpenMPI


