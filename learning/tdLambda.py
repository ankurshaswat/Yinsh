import sys

from gamePlay import GamePlayData,SingleGame
from utils import dotProduct,sigmoid,get_log_directory
import params

from tensorboardX import SummaryWriter



    
def log_weights(iteration, weights,writer):
    for i in range(len(weights)):
        writer.add_scalar("weights/"+params.features[i],weights[i],iteration)
    
   
    with open(params.logs_dir+"/weights.txt",'a+') as f:
        f.write(str(iteration)+"\n")
        f.write(str(weights)+"\n")



def isWinner(game,selfPlayer):
    if(len(game.stateValues)%2==0): #player 2 is winner
        return selfPlayer==1 
    else:
        return selfPlayer==0

    

def log_rms_error(epoch,gamePlayData,weights,selfPlayer,writer): # not sure if this is a useful metric
    errorinWin,errorinLoss=0,0
    for game in gamePlayData.gamesList:
        isWin=False
        if(isWinner(game,selfPlayer)):
            target=game.stateValues[-1]
            isWin=True
        else:
            target=game.stateValues[-2]
            isWin=False
        
        for featureValues in game.featureValuesList:
            if(isWin):
                errorinWin+=( (target-dotProduct(featureValues,weights))**2)
            else:
                errorinLoss+=( (target-dotProduct(featureValues,weights))**2)

    writer.add_scalar("rms_error/win",errorinWin,epoch)
    writer.add_scalar("rms_error/loss",errorinLoss,epoch)
    writer.add_scalar("rms_error/total",errorinLoss+errorinWin,epoch)
    

   
    
    
            
def tdUpdate(target,current,weights,features,learning_rate):
    delta=target-current
    delta*=((1-current)*current) #sigmoid gradient update
    # print("tdUpdate: ",target,current,delta)
    # print("\nWeights: ",["%0.2f" % i for i in weights])
    # print("\nFeatures: ",["%0.2f" % i for i in features])
    for i in range(len(weights)):
        weights[i]+=learning_rate*features[i]*delta
     


def train(gamePlayData, lamda=0.5,numEpochs=10,nstep=1, log_step=10,selfPlayer=0, learning_rate=0.01,rms_error_step=1):
    
    writer=SummaryWriter( log_dir=get_log_directory(params.comment) )

    weights=[i for i in gamePlayData.weights]
    print(len(weights))
    
    for epoch in range(numEpochs):
        print("EPOCH "+str(epoch)+"/"+str(numEpochs)+"\n\n")
        log_rms_error(epoch,gamePlayData,weights,selfPlayer, writer)
        for j in range(len(gamePlayData.gamesList)):
            game=gamePlayData.gamesList[j]
            prevVal=0
            for i in range(len(game.stateValues)-1,-1,-1): # In reverse order
                stateVal=game.stateValues[i]
                if i%2==selfPlayer:
                    if(i==len(game.stateValues)-1 or i==len(game.stateValues)-2): # terminal state (win/loss)
                        nextVal=stateVal
                        stateVal=sigmoid(dotProduct(game.featureValuesList[i],weights))
                    else:
                        nextVal=(1-lamda)*stateVal+lamda*(prevVal)
                    prevVal=nextVal

                    # update feature weights
                    tdUpdate(nextVal,stateVal,weights,game.featureValuesList[i],learning_rate)
                
                # print("Press")
                # sys.stdin.read(1)
            

            if(j%10==0 and j!=0):
                # log feature weights
                print("Episode "+str(j)+"/"+str(len(gamePlayData.gamesList)))
                # print(weights)
                log_weights(epoch*len(gamePlayData.gamesList)+j,weights,writer)
    
    print("Finished ")




