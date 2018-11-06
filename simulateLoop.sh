#!/bin/bash
#./simulateLoop.sh infRun4 &> simulations.log
mkdir gameRes
make
i=1
while :
  do 
    echo "Game $i"
    time ./yinsh 1 $1 0 0 "weights.txt" 1
    let "i += 1"
    echo " "
  done