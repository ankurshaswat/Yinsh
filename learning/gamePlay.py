import os
import params
from math import exp

## Helper Functions
def dotProduct(features,weights):
    ans=0
    for i in range(len(features)):
        ans+=features[i]*weights[i]
    return ans
def sigmoid(x):
    "Numerically-stable sigmoid function."
    if x >= 0:
        z = exp(-x)
        return 1 / (1 + z)
    else:
        z = exp(x)
        return z / (1 + z)
############



class SingleGame:
    def __init__(self):
        self.movesList=[]
        self.featureValuesList=[]
        self.stateValues=[]


class GamePlayData:
    def __init__(self,folderPath='./run1'):
        self.params=self.loadParams(folderPath+'/params.txt') # parameters like number of games, policy followed by self & opp during play
        self.numberOfGames=self.params['n']
        self.weights=self.loadWeights(folderPath+'/weights.txt') # weights with which these games were run        
        self.gamesList=self.loadGameData(folderPath+'./games.txt')


    def loadParams(self,filePath):
        ret={}
        with open(filePath,'r') as f:
            data=f.readlines()
            data=[i.rstrip('\n') for i in data]
            for line in data:
                x=line.split(' ')
                parameter=x[0]
                val=x[1]
                ret[parameter]=val
        for i in params.paramList:
            assert (i in ret.keys())
        return ret

    def loadWeights(self,filePath):
        ret=[]
        with open(filePath,'r') as f:
            data=f.readlines()
            data=[i.rstrip('\n') for i in data]
            ret=[float(i) for i in data[0].split(' ')]
            
        assert(len(ret)==len(params.features))
        return ret

    def loadGameData(self,filePath):
        numWeights=len(self.weights)
        ret=[]
        with open(filePath,'r') as f:
            data = f.readlines()
            data=[i.rstrip('\n') for i in data]
            for i in range(self.numberOfGames):
                newGame=SingleGame()
                counter=0
                while data[counter]!='END':
                    newGame.movesList.append(data[counter])
                    counter+=1
                    featureValues=[float(i) for i in data[counter].split(' ')]
                    
                    if(counter%4==3):
                        #player 2's move, so features need to be interchanged
                        t=len(featureValues)
                        featureValues=featureValues[t/2:]+featureValues[:t/2]


                    newGame.featureValuesList.append(featureValues)
                    newGame.stateValues.append(sigmoid(dotProduct(featureValues,self.weights)) )
                    

                    if(data[counter+1]=='END'):
                        #Victory & loss score
                        newGame.stateValues[:-1]=1.0+ 0.01*newGame.featureValuesList[:-1][params.features.index("OppRingsCount")]
                        newGame.stateValues[:-2]=0.0
                        

                    assert(len(newGame.featureValuesList[:-1]) == numWeights )
                    counter+=1
                ret.append(newGame)
        return ret
    
    


