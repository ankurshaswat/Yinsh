#!/bin/bash
#./runGames.sh ./run.sh ./run.sh 2
timee=$(date +%Y_%m_%d__%H_%M_%S)
mkdir selfPlay_$timee
source env/bin/activate
COUNTER=0
while [  $COUNTER -lt $3 ]; 
    do
        python server.py 10000 -n 5 -NC 2 -TL 150 -LOG selfPlay_$timee/gameRun_straight.log > selfPlay_$timee/serverData_runA$COUNTER.log &
        sleep 1
        python client.py 0.0.0.0 10000 $1 -mode GUI > selfPlay_$timee/script1_runA$COUNTER.log &
        sleep 1
        python client.py 0.0.0.0 10000 $2 > selfPlay_$timee/script2_runA$COUNTER.log

        python server.py 10000 -n 5 -NC 2 -TL 150 -LOG selfPlay_$timee/gameRun_inverted.log > selfPlay_$timee/serverData_runB$COUNTER.log &
        sleep 1
        python client.py 0.0.0.0 10000 $2 -mode GUI > selfPlay_$timee/script2_runB$COUNTER.log &
        sleep 1
        python client.py 0.0.0.0 10000 $1 > selfPlay_$timee/script1_runB$COUNTER.log

        let COUNTER=COUNTER+1 
    done
python statsGen.py selfPlay_$timee $1 $2
deactivate