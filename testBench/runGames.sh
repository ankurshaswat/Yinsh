#!/bin/bash
#./runGames.sh ./run.sh ./run.sh 2
mkdir selfPlay
source env/bin/activate
COUNTER=0
while [  $COUNTER -lt $3 ]; 
    do
        python server.py 10000 -n 5 -NC 2 -TL 150 -LOG selfPlay/gameRun.log > selfPlay/serverData_run$COUNTER.log &
        sleep 1
        python client.py 0.0.0.0 10000 $1 -mode GUI > selfPlay/player1_run$COUNTER.log &
        sleep 1
        python client.py 0.0.0.0 10000 $2 > selfPlay/player2_run$COUNTER.log
        let COUNTER=COUNTER+1 
    done
deactivate