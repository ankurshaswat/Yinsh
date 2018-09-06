from game import Game
import random
import sys
import time
import ast
import requests


class floydPlayer:

    def __init__(self):
        data = sys.stdin.readline().strip().split() # Initialize Environment
        self.player = int(data[0]) - 1 # player can have value 1 only
        self.n = int(data[1]) # n can have value 5 only
        self.time_left = int(data[2])
        self.game = Game(self.n)
        self.RingPos = {}
        self.play()
    def play_move_seq(self, move_seq):
        moves = ' '.join(move_seq) + '\n'
        sys.stdout.write(moves)
        sys.stdout.flush()

    def placeRing(self, hexagon, position):
        movetype = 'P'
        if hexagon==self.n and position%self.n==0:
            position+=1
        return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position), len(self.RingPos)

    def selectRing(self,ring_num):
        movetype = 'S'
        ring = self.RingPos[ring_num]
        return '{type} {hex} {pos}'.format(type=movetype, hex=ring[0], pos=ring[1])

    def moveRing(self,hexagon,position):
        movetype = 'M'
        if hexagon==self.n and position%self.n==0:
            position+=1
        return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position)





    def getResponseFromServer(self,gameRep, turnMode):
        board=[]
        for i in gameRep['ringsB']:
            board.append((i,'Ring B'))
        for i in gameRep['ringsA']:
            board.append((i,'Ring W'))
        for i in gameRep['markersB']:
            board.append((i,'Marker B'))
        for i in gameRep['markersA']:
            board.append((i,'Marker W'))

        board=(str(board)).replace("'","")


        query_string='GameState {activePlayer = W, turnMode = '+turnMode+', board = Board {bmap = fromList '
        query_string+=str(board)
        query_string+=', ringsB ='+ str(gameRep['ringsB'])
        query_string+=', ringsW = '+ str(gameRep['ringsA'])
        query_string+=', markersB = '+ str(gameRep['markersB'])
        query_string+=', markersW = '+ str(gameRep['markersA'])
        pointsB,pointsA=0,0
        if(turnMode!='AddRing'):
            pointsB=5-len(gameRep['ringsB'])
            pointsA=5-len(gameRep['ringsA'])
            
        query_string+='}, pointsB = '+str(pointsB)+', pointsW = '+str(pointsA)+'}'

        # Send GET request with query_string, and return parsed response

        url='https://yinsh-backend.herokuapp.com/'

        # payload={'gamestate':'GameState {activePlayer = W, turnMode = AddRing, board = Board {bmap = fromList [((1,2),Ring B)], ringsB = [(1,2)], ringsW = [], markersB = [], markersW = []}, pointsB = 0, pointsW = 0}'}
        payload={'gamestate':query_string}
        response=(requests.get(url,params=payload)).text

        print('# query_string: '+query_string)
        print('# Server response: '+response)

        nextGameRep={}
        response=response.split('ringsB = ')[1]
        nextGameRep['ringsB']=ast.literal_eval( (response.split('],')[0])+']' )

        response=response.split('ringsW = ')[1]
        nextGameRep['ringsA']=ast.literal_eval( (response.split('],')[0])+']' )

        response=response.split('markersB = ')[1]
        nextGameRep['markersB']=ast.literal_eval( (response.split('],')[0])+']' )

        response=response.split('markersW = ')[1]
        nextGameRep['markersA']=ast.literal_eval( (response.split(']},')[0])+']' )

        return nextGameRep
    
    def axial2hex(self,pos):
        a,b=pos[0],pos[1]
        hex, pos=0,0

        if (a==0 and b==0):
            hex=0
            pos=0
        elif (a <= 0 and b >= 0):
            hex = b - a
            pos = (5 * hex + b) % (6 * hex)
        
        elif (a >= 0 and b <= 0):
            hex = a - b
            pos = 3 * hex - a
        
        elif (a >= 0 and b >= 0):
            if (b > a):
                hex = b
                pos = a
            else:
                hex = a
                pos = 2 * hex - b
        else:
            if (b > a):
                hex = -a
                pos = 5 * hex + b
            else:
                hex = -b
                pos = 3 * hex - a
            
        return (hex,pos)

    def convertToMoveSeq(self,nextGameRep, currGameRep, turnMode):
        ringsBefore=currGameRep['ringsA'] 
        ringsAfter=nextGameRep['ringsA']
        if(turnMode=='AddRing'):
            hex,position=-1,-1
            for pos in ringsAfter:
                if pos not in ringsBefore:
                    hex,position=self.axial2hex(pos)
            moveP, i = self.placeRing(hex,position)
            x = self.game.execute_move(moveP)
            self.RingPos[i] = (hex, position)
            return [moveP]
        elif(turnMode=='AddMarker'):
            if(len(ringsBefore)==len(ringsAfter) ):
                # Ring selected and moved 
                index=-1
                for posf in ringsAfter:
                    for posi in ringsBefore:
                        if posf not in ringsBefore:
                            hex,position=self.axial2hex(posf)
                        if posi not in ringsAfter:
                            for i in self.RingPos.keys():
                                if self.RingPos[i]==self.axial2hex(posi):
                                    index=i
                                    break

                moveS = self.selectRing(index)
                moveM= self.moveRing(hex,position)
                self.RingPos[index]=(hex,position)
                x=self.game.execute_move(moveS)
                x=self.game.execute_move(moveM)
                return [moveS,moveM]
            else:
                # Row of 5 + ring removed
                pass

        return []

    def play(self):
        move = sys.stdin.readline().strip()
        self.game.execute_move(move)
        rings_placed=0
        while True: # Keep playing moves till game is ove
            currGameRep=self.game.getGameRep()
            if(rings_placed<self.n):
                rings_placed+=1
                turnMode='AddRing'
            else:
                turnMode='AddMarker'
            nextGameRep=self.getResponseFromServer(currGameRep,turnMode) 
            
            move_seq = []
            move_seq=self.convertToMoveSeq(nextGameRep,currGameRep,turnMode)
            
            self.play_move_seq(move_seq)

            ## Execute Other Player Move Sequence
            move = sys.stdin.readline().strip()
            self.game.execute_move(move)


floyd_player = floydPlayer()
