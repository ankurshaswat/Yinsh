import os
import sys
import params
from math import exp

from utils import dotProduct,sigmoid




class SingleGame:
    def __init__(self):
        self.movesList=[]
        self.featureValuesList=[]
        self.stateValues=[]


class GamePlayData:
    def __init__(self,folderPath='./run1'):
        self.params=self.loadParams(folderPath+'/params.txt') # parameters like number of games, policy followed by self & opp during play
        self.numberOfGames=int(self.params['n'])
        self.weights=self.loadWeights(folderPath+'/weights.txt', scale=True) # weights with which these games were run        
        self.gamesList=self.loadGameData(folderPath+'/games.txt')


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

    def loadWeights(self,filePath,scale=True):
        if(scale):
            scale_factor=params.scaling_factor
        ret=[]
        
        with open(filePath,'r') as f:
            data=f.readlines()
            data=[i.rstrip('\n') for i in data]
            ret=[scale_factor*float(i) for i in data[0].split(' ')]
            ret=ret[1:] #first element is num elements,so discard
            
        try:
            assert(len(ret)==len(params.features))
        except AssertionError:
            print(len(ret))
            print(len(params.features))
            sys.exit(0)
        
        return ret

    def loadGameData(self,filePath):
        numWeights=len(self.weights)
        ret=[]
        with open(filePath,'r') as f:
            data = f.readlines()

            # cleanup
            data=[i.rstrip(' \n') for i in data]
            data=[i.rstrip('\n') for i in data]
            
            for i in range(self.numberOfGames):
                newGame=SingleGame()
                counter=0
                while data[counter]!='END':
                    newGame.movesList.append(data[counter])
                    counter+=1
                    try:
                        featureValues=[float(i) for i in data[counter].split(' ')]
                    except:
                        print([i for i in data[counter].split(' ')])
                        sys.exit(0)

                    if(counter%4==3):
                        #player 2's move, so features need to be interchanged
                        t=len(featureValues)
                        featureValues=featureValues[t/2:]+featureValues[:t/2]


                    newGame.featureValuesList.append(featureValues)
                    newGame.stateValues.append(sigmoid( dotProduct(featureValues,self.weights) ) )
                    

                    if(data[counter+1]=='END'):
                        #Victory & loss score
                        newGame.stateValues[-1]=1.0 #+ 0.01*newGame.featureValuesList[ -1][params.features.index("OppRingsCount")]
                        newGame.stateValues[-2]=0.0
                        
                    try:
                        assert(len(newGame.featureValuesList[-1]) == numWeights )
                    except AssertionError:
                        print(len(newGame.featureValuesList[-1]) )
                        print(numWeights)
                        sys.exit(0)
                    
                    counter+=1
                ret.append(newGame)
        return ret
    
    


