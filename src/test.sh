#!/bin/bash

program="./openmp/wave"
configFile="./openmp/wave.conf"
resultsFile="results.txt"
grepValue="real"

function calculateAverage {
    for i in `seq 1 10`;
    do
        timeOutput=$( { time -p OMP_NUM_THREADS=$1 $program $2 $3 $4; } 2>&1 )
        result=$(echo "$timeOutput" | grep $grepValue)
        resultTime=${result:5:4}
        resultTimes[$i]=$resultTime
        echo $resultTime
    done

    sum=0
    for i in "${resultTimes[@]}"
    do
        sum=$(echo $sum + $i | bc -l);
    done

    average=$(echo $sum / ${#resultTimes[@]} | bc -l)
    echo "$1 $3 $4: ${average:0:9}" >> $resultsFile
}

#calculateAverage <OMP_NUM_THREADS> <CONFIG_FILE> <ARRAY_SIZE> <STEP_SIZE>

echo "OMP_NUM_THREADS=1"
threads="1"
simulations=1000
calculateAverage $threads $configFile 0 $simulations
calculateAverage $threads $configFile 500000 $simulations
calculateAverage $threads $configFile 1000000 $simulations
calculateAverage $threads $configFile 5000000 $simulations
calculateAverage $threads $configFile 10000000 $simulations
calculateAverage $threads $configFile 15000000 $simulations
calculateAverage $threads $configFile 20000000 $simulations

echo "OMP_NUM_THREADS=2"
threads="2"
simulations=1000
calculateAverage $threads $configFile 0 $simulations
calculateAverage $threads $configFile 500000 $simulations
calculateAverage $threads $configFile 1000000 $simulations
calculateAverage $threads $configFile 5000000 $simulations
calculateAverage $threads $configFile 10000000 $simulations
calculateAverage $threads $configFile 15000000 $simulations
calculateAverage $threads $configFile 20000000 $simulations