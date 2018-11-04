from gamePlay import GamePlayData,SingleGame
import params

def tdUpdate(target,current,weights,features,learning_rate):
    delta=target-current
    delta*=((1-current)*current) #sigmoid gradient update
    for i in range(len(weights)):
        weights[i]+=learning_rate*features[i]*delta
    

def train(gamePlayData, lamda=0.5,numEpochs=1,nstep=1, log_step=10,selfPlayer=0, learning_rate=0.01):
    weights=[i for i in gamePlayData.weights]
    for epoch in numEpochs:
        print("EPOCH "+str(epoch)+"/"+str(numEpochs)+"\n\n")
        for j in gamePlayData:
            game=gamePlayData[j]
            prevVal=0
            for i in range(len(game.stateValues)-1,-1,-1): # In reverse order
                stateVal=game.stateValues[i]
                if i%2==selfPlayer:
                    if(i==len(game.stateValues)-1): # terminal state
                        nextVal=stateVal
                    else:
                        nextVal=(1-lamda)*stateVal+lamda*(prevVal)
                    prevVal=nextVal

                    # update feature weights
                    tdUpdate(nextVal,stateVal,weights,game[stateVal].featureValues,learning_rate)
            if(j%10==0 and i!=0):
                # log feature weights
                print("Episode "+str(j)+"/"+str(len(gamePlayData)))
                print(weights)

    print("Finished ")

