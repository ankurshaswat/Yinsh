import sys
from game import Game
from flask import Flask,request
import logging

app = Flask(__name__)
log = logging.getLogger('werkzeug')
log.disabled = True
app.logger.disabled = True


@app.route('/',methods = ['POST'])
def play():
    global game,player,last_move
    
    move_player = str(request.form['player'])
    if(move_player != str(player)):
        return "other player move"

    # store human move
    move = ''
    t = str(request.form['type']).strip().split()
    x = str(request.form['x']).strip().split()
    y = str(request.form['y']).strip().split()
    
    n = len(t)
    
    for i in range(n):
        # remove multiple selects
        if(i<n-1):
            if t[i] == t[i+1]:
                continue
        x[i],y[i] = vector_to_hex(int(x[i]),int(y[i]))
        move += t[i] + " " + str(x[i]) + " " + str(y[i])
        move += " "
    
    # check multiple req
    if(move == last_move):
        return
    last_move = move
    # # send the move to opponent
    print(move)
    sys.stdout.flush()
    return "yes"
####################
def vector_to_hex(x,y):

    # // divide the board into six segments using 3 axes
    # // x=0 y=0 x=y
    cx,cy = 5,5
    x,y = x-cx,y-cy
    
    h,p = 0,0
    # // for point on any axis
    if (x == y or x == 0 or y == 0):
        if (x == y):
            if (x >= 0):
            
                h = x
                p = x
            
            else:
            
                x = -x
                h = x
                p = 4 * x
            
        
        elif (x == 0):
        
            if (y >= 0):
            
                h = y
                p = 0
            
            else:
            
                y = -y
                h = y
                p = 3 * y
            
        
        else:
        #  // y==0
            if (x >= 0):
                h = x
                p = 2 * x
            
            else:
                x = -x
                h = x
                p = 5 * x
            
        
    
    elif (x > y):
    
        # // SW side
        if (x < 0 and y < 0):
        
            y = -y
            h = y
            p = 3 * y - x
        
        # // SE side
        elif (x > 0 and y < 0):
        
            h = x - y
            p = 2 * (x - y) - y
        
        # // E side
        else:
        
            h = x
            p = x + (x - y)
        
    
    else:
    
        # // W side
        if (x < 0 and y < 0):
        
            x = -x
            h = x
            p = 4 * x + (x + y)
        
        # // NW side
        elif (x < 0 and y > 0):
        
            x = -x
            h = x + y
            p = 5 * (x + y) + y
        
        # //NE side
        else:
        
            h = y
            p = x
        
    return h,p


###################
## initialize
data = sys.stdin.readline().strip().split() # Initialize Environment
player = 0#int(data[0]) - 1 # player can have values 0 and 1
n = 5#int(data[1]) # n can have values 5, 6, or 7
time_left = 120#int(data[2])
seq = 5 #int(data[3])
##
last_move = ''
app.run(port=5001)