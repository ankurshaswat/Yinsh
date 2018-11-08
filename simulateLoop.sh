#!/bin/bash
#./simulateLoop.sh infRun4 &> simulations.log
timee=$(date +%Y_%m_%d__%H_%M_%S)
mkdir gameRes
make
i=1
while :
  do 
    echo "Game $i"
    time ./yinsh 1 $1$timee 0 0 "weights.txt" 1
    let "i += 1"
    echo " "
  done