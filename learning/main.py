import os
import sys
import argparse

import params

from gamePlay import GamePlayData
from tdLambda import train

if __name__ == '__main__':
        #initialize default parameters
    params.init() 



    #command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--logs_dir', type=str, default=params.logs_dir)
    parser.add_argument('--games_dir', type=float, default=params.games_dir)
     
    parser.add_argument('--comment', type=str, default=params.comment)

    parser.add_argument('--num_epochs', type=int, default=params.num_epochs)
    parser.add_argument('--learning_rate', type=float, default=params.learning_rate)
    parser.add_argument('--lamda', type=float, default=params.lamda)
    parser.add_argument('--scale_factor', type=float, default=params.scaling_factor)


    # Load game data
    gamePlayData=GamePlayData(params.games_dir)
   
    # Train
    train(gamePlayData,params.lamda,params.num_epochs)