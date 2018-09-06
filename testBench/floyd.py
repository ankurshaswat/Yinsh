from game import Game
import random
import sys
import time

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



	def play(self):
        move = sys.stdin.readline().strip()
        self.game.execute_move(move)

		while True: # Keep playing moves till game is ove
            currGameRep=self.game.getGameRep()
            if(rings_placed<self.n):
                rings_placed+=1
                turnMode='AddRing'
            else
                turnMode='AddMarker'
            nextGameRep=getResponseFromServer(currGameRep,turnMode) 
            
            move_seq = []
            move_seq=convertToMoveSeq(nextGameRep,currGameRep,turnMode)
			
            self.play_move_seq(move_seq)

			## Execute Other Player Move Sequence
			move = sys.stdin.readline().strip()
			self.game.execute_move(move)


    def getResponseFromServer(gameRep, turnMode):
        query_string='GameState {activePlayer = W, turnMode = '+turnMode+', board = Board {bmap = fromList '
        query_string+=board
        query_string+=', ringsB ='+ str(gameRep['ringsB'])
        query_string+=', ringsW = '+ str(gameRep['ringsA'])
        query_string+=', markersB = '+ str(gameRep['markersB'])
        query_string+=', markersW = '+ str(gameRep['markersA'])
        pointsB,pointsA=0,0
        if(turnMode!='AddMarker'):
            pointsB=self.n-len(gameRep['ringsB'])
            pointsA=self.n-len(gameRep['ringsA'])
            
        query_string+='}, pointsB = '+str(pointsB)+', pointsW = '+str(pointsA)+'}'

        # Send GET request with query_string, and return parsed response
        nextGameRep={}

        return nextGameRep
    
    def translateMove(pos):
        x=pos[0],y=pos[1]
        # todo
        return (x,y)

    def convertToMoveSeq(nextGameRep, currGameRep, turnMode):
        ringsBefore=currGameRep['ringsB'], ringsAfter=nextGameRep['ringsB']
        if(turnMode=='AddRing'):
            hex,position=-1
            for pos in ringsAfter:
                if pos not in ringsBefore:
                    hex,position=translateMove(pos)
            moveP, i = self.placeRing(hex,position)
            x = self.game.execute_move(moveP)
            self.RingPos[i] = (hex, position)
            return [moveP]
        elif(turnMode=='AddMarker'):
            if(len(ringsBefore)==len(ringsAfter) ):
                # Select a ring and move to valid position
                index=-1
                for posf,posi in ringsAfter,ringsBefore:
                    if posf not in ringsBefore:
                        hex,position=translateMove(posf)
                    if posi not in ringsAfter:
                        for i in self.RingPos.keys():
                            if self.RingPos[index]==j:
                                index=i
                                break

                moveS = self.selectRing(index)
                moveM= self.moveRing(hex,position)
                x=self.game.execute_move(moveS)
                x= = self.game.execute_move(moveM)
                return [moveS,moveM]
            else:
                pass

        return []


floyd_player = floydPlayer()
