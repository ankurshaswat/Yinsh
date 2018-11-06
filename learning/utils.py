import os
import params
from math import exp
from datetime import datetime
import socket

## Helper Functions
def dotProduct(features,weights):
    ans=0.0
    for i in range(len(features)):
        ans+=features[i]*weights[i]
    return ans

def sigmoid(x):
    "Numerically-stable sigmoid function."
    if x >= 0:
        z = exp(-x)
        ans= 1 / (1 + z)
    else:
        z = exp(x)
        ans= z / (1 + z)
    # print("Sigmoid: ",z)
    return ans
    
    

def get_log_directory(comment=""):
    current_time = datetime.now().strftime('%b%d_%H-%M-%S')
    log_dir = os.path.join(params.logs_dir, current_time + '_' + socket.gethostname() + comment)
    return log_dir

############
