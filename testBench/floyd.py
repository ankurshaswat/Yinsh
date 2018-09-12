from game import Game
import random
import sys
import time
import ast
import requests
import copy

PositionStates={'whiteMarker':-1,'blackMarker':1,'whiteRing':-2,'blackRing':2,'empty':0}
MoveType = {'removeRow':2,'placeRing':0,'moveRing':1,'removeRing':3}
class Pair:

    def __init__(self,i,j):
        self.first = i
        self.second = j

    def copy(self):
        return Pair(self.first,self.second)

class Move:

    def __init__(self,moveType,initPosition,finalPosition):
        self.moveType = moveType
        self.initPosition = initPosition
        self.finalPosition = finalPosition

    def copy(self):
        return Move( self.moveType, self.initPosition.copy(), self.finalPosition.copy())

class floydPlayer:

    def __init__(self):
        data = sys.stdin.readline().strip().split() # Initialize Environment
        self.player = int(data[0]) - 1 # player can have value 1 only
        self.n = int(data[1]) # n can have value 5 only
        self.time_left = int(data[2])
        self.game = Game(self.n)
        self.RingPos = {}
        self.play()
        self.board=[]
        self.prevMoveRing=Move(MoveType['moveRing'],Pair(-1,-1),Pair(-1,-1))

    def play_move_seq(self, move_seq):
        moves = ' '.join(move_seq) + '\n'
        sys.stdout.write(moves)
        sys.stdout.flush()

    def placeRing(self, hexagon, position):
        movetype = 'P'
        if hexagon==self.n and position%self.n==0:
            position+=1
        return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position), len(self.RingPos)

    def removeRing(self,ring_num):
        movetype = 'X'
        ring = self.RingPos[ring_num]
        return '{type} {hex} {pos}'.format(type=movetype, hex=ring[0], pos=ring[1])


    def selectRing(self,ring_num):
        movetype = 'S'
        ring = self.RingPos[ring_num]
        return '{type} {hex} {pos}'.format(type=movetype, hex=ring[0], pos=ring[1])

    def moveRing1(self,hexagon,position):
        movetype = 'M'
        if hexagon==self.n and position%self.n==0:
            position+=1
        return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position)

    def removeRowStart(self,hexagon,position):
        movetype = 'RS'
        if hexagon==self.n and position%self.n==0:
            position+=1
        return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position)

    def removeRowEnd(self,hexagon,position):
        movetype = 'RE'
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

    def convertMoveSeq(self,finalMoveSeq):
        ans=[]
        for move in finalMoveSeq:
            if(move.moveType==MoveType['removeRing']):
                hexagon,position=self.axial2hex((move.initPosition.first,move.initPosition.second))
                ans.append('{type} {hex} {pos}'.format(type='X', hex=hexagon, pos=position))
            elif(move.moveType==MoveType['moveRing']):
                hexagon,position=self.axial2hex((move.initPosition.first,move.initPosition.second))
                ans.append('{type} {hex} {pos}'.format(type='S', hex=hexagon, pos=position))
                hexagon,position=self.axial2hex((move.finalPosition.first,move.finalPosition.second))
                ans.append('{type} {hex} {pos}'.format(type='M', hex=hexagon, pos=position))
            elif(move.moveType==MoveType['removeRow']):
                hexagon,position=self.axial2hex((move.initPosition.first,move.initPosition.second))
                ans.append('{type} {hex} {pos}'.format(type='RS', hex=hexagon, pos=position))
                hexagon,position=self.axial2hex((move.finalPosition.first,move.finalPosition.second))
                ans.append('{type} {hex} {pos}'.format(type='RE', hex=hexagon, pos=position))
            else:
                print("# Unknown move ?! \n")

        for move in ans:
            self.game.execute_move(move)

        return ans

    def convertToMoveSeq(self,nextGameRep, currGameRep, turnMode):
        ringsBefore=currGameRep['ringsA'] 
        ringsAfter=nextGameRep['ringsA']
        # self.setBoard()
        # print("# Sanity check-")
        # _=self.sameBoard(currGameRep)
        
        if(turnMode=='AddRing'):
            hex,position=-1,-1
            for pos in ringsAfter:
                if pos not in ringsBefore:
                    hex,position=self.axial2hex(pos)
            moveP, i = self.placeRing(hex,position)
            x = self.game.execute_move(moveP)
            # self.RingPos[i] = (hex, position)
            return [moveP]
        elif(turnMode=='AddMarker'):
            rows=[]
            self.getValidRowMoves(rows,True)
            moveSeq=[]
            moveSequences=[]
            if(len(rows)==0):
                print("# Opponent has made no rows for you :( ")
                self.moveMarkerMoves(moveSeq,moveSequences)
            else:
                print("# Opponent has made a row :) ")
                self.rowMoves(rows,moveSeq,moveSequences,True)
            
            finalMoveSeq=[]
            print("# MoveSequences found- "+str(moveSequences))
            for moveSequence in moveSequences:
                for move in moveSequence:
                    self.playMove(move)
                # print("# Sanity check-")
                # _=self.sameBoard(currGameRep)
                if(self.sameBoard(nextGameRep)):
                    finalMoveSeq=moveSequence
                    break
                for move in reversed(moveSequence):
                    self.undoMove(move)

            ans=self.convertMoveSeq(finalMoveSeq)
            print("# Answer-"+str(ans))
            return ans                

    def sameBoard(self,nextGameRep):
        rings_A,rings_B,markers_A,markers_B=[],[],[],[]        
        for i in range(-1*self.n,self.n+1):
            for j in range(-1*self.n,self.n+1):
                if(self.getState(Pair(i,j))==PositionStates['whiteRing'] ):
                    rings_A.append(sorted( (i,j) ))
                elif(self.getState(Pair(i,j))==PositionStates['blackRing'] ):
                    rings_B.append(sorted( (i,j) ))
                elif(self.getState(Pair(i,j))==PositionStates['whiteMarker'] ):
                    markers_A.append(sorted( (i,j) ))
                elif(self.getState(Pair(i,j))==PositionStates['blackMarker'] ):
                    markers_B.append(sorted( (i,j) ))
        # print("# Sameboard: RingsA "+str(rings_A))
        # print("# Sameboard: RingsB "+str(rings_B) )
        # print("# Sameboard: markersA "+str(markers_A) )
        # print("# Sameboard: markersB "+str(markers_B) )


        for i in nextGameRep.keys():
            for j in range(len(nextGameRep[i])):
                nextGameRep[i][j]=sorted(nextGameRep[i][j]) #dirty code );
        
        if(sorted(nextGameRep['ringsA'])==sorted(rings_A) and sorted(nextGameRep['ringsB'])==sorted(rings_B)
            and sorted(nextGameRep['markersA'])==sorted(markers_A) and sorted(nextGameRep['markersB'])==sorted(markers_B) ):
            print("# SAME BOARD!! ")
            return True

        return                    
      

    def getValidRemoveRingMoves(self,moves):
        for i in range(-1*self.n,self.n+1):
            for j in range(-1*self.n,self.n+1):
                if(self.getState(Pair(i,j))==PositionStates['whiteRing'] ):
                    moves.append(Move(MoveType['removeRing'],Pair(i,j),Pair(-1,-1)))

        return

    def moveMarkerMoves(self,moveSeq, moveSequences):
        moveRingMoves=[]
        self.getValidRingMoves(moveRingMoves)
        print("# Valid Ring Moves-"+str(len(moveRingMoves)))
        for m in moveRingMoves:
            # play move m
            self.playMove(m)
            moveSeq.append(m.copy())
            removeRowMovesAgain=[]
            self.getValidRowMoves(removeRowMovesAgain, True)
            if (len(removeRowMovesAgain) > 0):
                self.rowMoves(removeRowMovesAgain, moveSeq, moveSequences, False)
            else:
                moveSequences.append(copy.deepcopy(moveSeq) )

            self.undoMove(m)

            moveSeq.pop()
        return 


    def getRingsCount(self):
        ans=0
        for i in range(-1*self.n,self.n+1):
            for j in range(-1*self.n,self.n+1):
                if(self.getState(Pair(i,j))==PositionStates['whiteRing'] ):
                    ans+=1
        return ans
       
    def isIntersecting(self,rowMove1,rowMove2):
        return True

    def rowMoves(self,removeRowMoves,moveSeq,moveSequences,continuePlaying):
        # ringsLeft = self.getRingsCount()

        for i in range(len(removeRowMoves) ):
            moveSeqFound = False
            # play remove row move
            self.playMove(removeRowMoves[i])
            moveSeq.append(removeRowMoves[i].copy())
            moveRingMoves=[]
            self.getValidRemoveRingMoves(moveRingMoves)
            for m1 in moveRingMoves:
                # play moveRing move
                self.playMove(m1)
                moveSeq.append(m1.copy())
                #cout << "# Row made after opponent's move- "
                #    << " " << board.getRingsCount(player) << " " << removeRowMoves[i].initPosition.first << ", " << removeRowMoves[i].initPosition.second << endl;

                for  j in range(i + 1,len(removeRowMoves)):
                    if (not self.isIntersecting(removeRowMoves[i], removeRowMoves[j])):
                        # play remove row move
                        self.playMove(removeRowMoves[j])
                        moveSeq.append(removeRowMoves[j].copy())
                        moveRingMoves2=[]
                        self.getValidRemoveRingMoves(moveRingMoves2)
                        for  m2 in moveRingMoves2:
                            # play moveRing move
                            self.playMove(m2)
                            moveSeq.append(m2.copy())
                            for  k  in range(j + 1,len(removeRowMoves)):
                                if (not( self.isIntersecting(removeRowMoves[k], removeRowMoves[j]) ) and not( self.isIntersecting(removeRowMoves[k], removeRowMoves[i]) ) ):
                                    # play remove row move
                                    self.playMove(removeRowMoves[k])
                                    moveSeq.append(removeRowMoves[k].copy())
                                    moveRingMoves3=[]
                                    self.getValidRemoveRingMoves(moveRingMoves3)
                                    for m3 in moveRingMoves3:
                                        moveSeq.append(m3.copy())
                                        # play moveRing move
                                        self.playMove(m3)
                                        # Evaluate new board position till shallow depth (for move ordering)
                                        moveSeqFound = True
                                        moveSequences.append(copy.deepcopy(moveSeq) ) #push this with evaluated value

                                        self.undoMove(m3)
                                        moveSeq.pop()
                                    self.undoMove(removeRowMoves[k])
                                    moveSeq.pop()
                            if (not moveSeqFound):
                                moveSeqFound = True
                                if (self.getRingsCount() == 2 or (not continuePlaying) ):
                                    # Evaluate new board position till shallow depth (for move ordering)
                                    moveSequences.append(copy.deepcopy(moveSeq)) #push this with evaluated value
                                else:
                                    self.moveMarkerMoves(moveSeq, moveSequences)

                            self.undoMove(m2)
                            moveSeq.pop()
                        self.undoMove(removeRowMoves[j])
                        moveSeq.pop()

                if (not moveSeqFound):
                    # Evaluate new board position till shallow depth (for move ordering)
                    moveSeqFound = True
                    if (self.getRingsCount() == 2 or (not continuePlaying) ):
                        # Evaluate new board position till shallow depth (for move ordering)
                        moveSequences.append(copy.deepcopy(moveSeq)) #push this with evaluated value
                    else:
                        self.moveMarkerMoves( moveSeq, moveSequences)

                self.undoMove(m1)
                moveSeq.pop()
            self.undoMove(removeRowMoves[i])
            moveSeq.pop()
        return










    def getValidRingMoves(self,moves):
        directions=[(0,1),(1,0),(1,1),(0,-1),(-1,0),(-1,-1)]
        directions=[Pair(i[0],i[1]) for i in directions]
        ringsA=[]
        for i in range(-1*self.n,self.n+1):
            for j in range(-1*self.n,self.n+1):
                if(self.getState(Pair(i,j))==PositionStates['whiteRing'] ):
                    ringsA.append((i,j))
        print("# Floyd rings- "+str(ringsA))
        for ringPos in ringsA:
            ringPosition = Pair(ringPos[0],ringPos[1])
            for j in range(len(directions)):
                direction =directions[j].copy()
                checkPosition = ringPosition.copy()

                jumpedMarker = False

                while (True):
                    checkPosition.first += direction.first
                    checkPosition.second += direction.second
                    if (not self.validPosition(checkPosition)):
                        break

                    positionState = self.getState(checkPosition)

                    jumpedMarker = jumpedMarker or (positionState == PositionStates['blackMarker']) or (positionState == PositionStates['whiteMarker'])

                    if (positionState == PositionStates['whiteRing'] or positionState == PositionStates['blackRing']):
                        break
                    elif (positionState == PositionStates['empty']):
                        moves.append(Move(MoveType['moveRing'], ringPosition.copy(), checkPosition.copy()))

                        if (jumpedMarker):
                            break
        return


    def playMove(self,m):
        type = m.moveType
        if (type == MoveType['placeRing']):
            self.setState(m.initPosition,PositionStates['whiteRing'])
        elif (type == MoveType['moveRing']):
            self.moveRing(m.finalPosition, m.initPosition,True)
        elif (type == MoveType['removeRing']):
            self.setState(m.initPosition, PositionStates['empty'])
        else:
            self.removeMarkers(m.initPosition, m.finalPosition)

    def undoMove(self,m):
        type = m.moveType
        if (type == MoveType['placeRing']):
            self.removeRing(m.initPosition)
        elif (type == MoveType['moveRing']):
            self.setState(m.initPosition, PositionStates['empty'])
            self.moveRing(m.initPosition, m.finalPosition, True)
            self.setState(m.finalPosition, PositionStates['empty'])
        elif (type == MoveType['removeRing']):
            self.setState(m.initPosition,PositionStates['whiteRing'])
        else:
            # cout << "# Undo Remove Row??? (" << m.finalPosition.first << ',' << m.finalPosition.second << ") (" << m.initPosition.first << ',' << m.initPosition.second << ")" << endl;
            self.placeMarkers(m.initPosition, m.finalPosition)

    def placeMarkers(self,startSeries,endSeries):
        if (startSeries.first == endSeries.first):
            increment = -1 if(startSeries.second < endSeries.second) else 1
            for i in range(endSeries.second,startSeries.second + increment,increment):
                self.setState(Pair(endSeries.first, i), PositionStates['whiteMarker'])
        elif (startSeries.second == endSeries.second):
            increment = -1 if(startSeries.first < endSeries.first) else 1
            for i in range(endSeries.first,startSeries.first + increment,increment):
                self.setState(Pair(i, endSeries.second), PositionStates['whiteMarker'])
        else:
            increment = -1 if(startSeries.second < endSeries.second) else 1
            i2 = endSeries.second
            for i1 in range(endSeries.first,startSeries.first + increment,increment):
                self.setState(Pair(i1, i2), PositionStates['whiteMarker'])
                i2 += increment
        return 
        

    def removeMarkers(self,startSeries,endSeries):
        if (startSeries.first == endSeries.first):
            increment = -1 if(startSeries.second < endSeries.second) else 1
            for i in range(endSeries.second,startSeries.second + increment,increment):
                self.setState(Pair(endSeries.first, i), PositionStates['empty'])
        elif (startSeries.second == endSeries.second):
            increment = -1 if(startSeries.first < endSeries.first) else 1
            for i in range(endSeries.first,startSeries.first + increment,increment):
                self.setState(Pair(i, endSeries.second), PositionStates['empty'])
        else:
            increment = -1 if(startSeries.second < endSeries.second) else 1
            i2 = endSeries.second
            for i1 in range(endSeries.first,startSeries.first + increment,increment):
                self.setState(Pair(i1, i2), PositionStates['empty'])
                i2 += increment
        return 
    


    def play(self):
        move = sys.stdin.readline().strip()
        self.game.execute_move(move)
        rings_placed=0
        while True: # Keep playing moves till game is ove
            self.board,currGameRep=self.game.getGameRep()
            self.board=copy.deepcopy(self.board)
            # print("#Board state: "+str(self.board))
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

    def validPosition(self,position):
        absPos1 = abs(position.first)
        absPos2 = abs(position.second)
        if (position.first == 0 or position.second == 0 or position.first == position.second):
            return (absPos1 < self.n and absPos2 < self.n)
        elif (position.first * position.second > 0):
            return (absPos1 <= self.n and absPos2 <= self.n)
        else:
            return (absPos1 + absPos2 <= self.n)

    # def setBoard(self):
    #     self.board=copy.deepcopy(self.game.getGameBoard())


    def setState(self,position,state):
        self.board[position.first + self.n][position.second + self.n] = state


    def getState(self,position):
        return self.board[position.first + self.n][position.second + self.n]

    def invertState(self,p1,p2):
        # print("# invertState "+str(p1+self.n))
        # print("# invertState "+str(p2+self.n) )
        # print("# invertState "+str( len(self.board) )) 
        # print("# invertState "+str( len(self.board[0]) ))   
          
        self.board[p1 + self.n][p2 + self.n] = -1 * self.board[p1 + self.n][p2 + self.n]

    def moveRing(self,newPosition,currentPosition,player):

        playerRing = PositionStates['whiteRing'] if player else PositionStates['blackRing']
        playerMarker = PositionStates['whiteMarker'] if player else PositionStates['blackMarker']
        self.setState(currentPosition, playerMarker)
        self.setState(newPosition, playerRing)

        if (newPosition.first == currentPosition.first):
            increment = PositionStates['whiteMarker'] if newPosition.second < currentPosition.second else 1
            for i in range(currentPosition.second + increment,newPosition.second,increment):
                self.invertState(currentPosition.first, i)
        elif (newPosition.second == currentPosition.second):
            increment = -1 if newPosition.first < currentPosition.first else 1
            for  i in range(currentPosition.first + increment,newPosition.first,increment):
                self.invertState(i, currentPosition.second)
        else:
            increment = -1 if newPosition.second < currentPosition.second else 1
            i1 = currentPosition.first + increment
            i2 = currentPosition.second + increment
            while(i1 != newPosition.first):
                # print("# Calling invert state: "+str(i1)+" "+str(i2)+" "+str(len(self.board))+" "+str(len(self.board[0])) ) 
                self.invertState(i1, i2)
                i1 += increment
                i2 += increment

        # ringIndex = 1 if player else 0
        # for i in range(0,rings[ringIndex].size()):
        #     pos = rings[ringIndex][i].copy()
        #     if (pos.first == currentPosition.first and pos.second == currentPosition.second):
        #         rings[ringIndex][i] = newPosition.copy()
        #         break
        return True

    def checkMarkersLocal(self,position, direction, player):
        playerMarker = PositionStates['whiteMarker'] if player else PositionStates['blackMarker']

        if (self.getState(position) == playerMarker):
            count = 1

            startMarker = position.copy()
            startMarker.first += direction.first
            startMarker.second += direction.second

            while (self.validPosition(startMarker)):
                count += 1
                # if (self.getState(startMarker) != playerMarker):
                if (self.getState(startMarker) != playerMarker or count>5):
                    count -= 1
                    break

                startMarker.first += direction.first
                startMarker.second += direction.second
            startMarker.first -= direction.first
            startMarker.second -= direction.second

            if (count >= 5):
                return Pair(True, Pair(position.copy(), startMarker.copy()))

            # endMarker = position.copy()
            # endMarker.first -= direction.first
            # endMarker.second -= direction.second

            # while (self.validPosition(endMarker)):
            #     count += 1
            #     if (self.getState(endMarker) != playerMarker):
            #         count -= 1
            #         break
            #     endMarker.first -= direction.first
            #     endMarker.second -= direction.second
            # endMarker.first += direction.first
            # endMarker.second += direction.second

            # if (count >= 5):
            #     return Pair(True, Pair(startMarker.copy(), endMarker.copy()))

        return Pair(False, Pair(position.copy(), position.copy()))

    def checkMarkersLine(self,position, direction, player):
        
        combinationSequences = []
        playerMarker = PositionStates['whiteMarker'] if player else PositionStates['blackMarker']

        newStartMarker = position.copy()
        newStartMarker.first += direction.first
        newStartMarker.second += direction.second

        while (self.validPosition(newStartMarker)):
            if (self.getState(newStartMarker) != playerMarker):
                newStartMarker.first += direction.first
                newStartMarker.second += direction.second
            else:
                count = 1
                startMarker = newStartMarker.copy()
                endMarker = startMarker.copy()
                endMarker.first += direction.first
                endMarker.second += direction.second
                while (self.validPosition(endMarker)):
                    count += 1
                    if (self.getState(endMarker) != playerMarker):
                        count -=1
                        break
                    endMarker.first += direction.first
                    endMarker.second += direction.second
                endMarker.first -= direction.first
                endMarker.second -= direction.second

                if (count >= 5):
                    combinationSequences.append(Pair(startMarker.copy(), endMarker.copy()))

                newStartMarker.first = endMarker.first + 2 * direction.first
                newStartMarker.second = endMarker.second + 2 * direction.second

        return combinationSequences

    def checkMarkers(self,newPosition, oldPosition, player):
        combinationSequences = []

        if (newPosition.first == oldPosition.first and newPosition.second == oldPosition.second):
            return combinationSequences

        returnedSequence = []

        if (newPosition.first == oldPosition.first):
            increment = -1 if newPosition.second < oldPosition.second else 1

            combinationSequences = self.checkMarkersLine(newPosition.copy(), Pair(0, increment * -1), player)

            for i in range(oldPosition.second,newPosition.second,increment):
                returnedSequence = self.checkMarkersLocal(Pair(newPosition.first, i), Pair(1, 0), player)
                if (returnedSequence.first == True):
                    combinationSequences.append(returnedSequence.second.copy())

                returnedSequence = self.checkMarkersLocal(Pair(newPosition.first, i), Pair(1, 1), player)
                if (returnedSequence.first == True):
                    combinationSequences.append(returnedSequence.second.copy())
        elif (newPosition.second == oldPosition.second):
            increment = -1 if newPosition.first < oldPosition.first else 1

            combinationSequences = self.checkMarkersLine(newPosition.copy(), Pair(increment * -1, 0), player)

            for i in range(oldPosition.first,newPosition.first,increment):
                returnedSequence = self.checkMarkersLocal(Pair(i, newPosition.second), Pair(0, 1), player)
                if (returnedSequence.first == True):
                    combinationSequences.append(returnedSequence.second.copy())

                returnedSequence = self.checkMarkersLocal(Pair(i, newPosition.second), Pair(1, 1), player)
                if (returnedSequence.first == True):
                    combinationSequences.append(returnedSequence.second.copy())
        else:
            increment = -1 if newPosition.second < oldPosition.second else 1

            combinationSequences = self.checkMarkersLine(newPosition.copy(), Pair(increment * -1, increment * -1), player)

            i1 = oldPosition.first
            i2 = oldPosition.second

            while(i1 != newPosition.first):
                returnedSequence = self.checkMarkersLocal(Pair(i1, i2), Pair(0, 1), player)
                if (returnedSequence.first == True):
                    combinationSequences.append(returnedSequence.second.copy())

                returnedSequence = self.checkMarkersLocal(Pair(i1, i2), Pair(1, 0), player)
                if (returnedSequence.first == True):
                    combinationSequences.append(returnedSequence.second.copy())
                
                i1 += increment
                i2 += increment

        return combinationSequences

    def inclusiveMarkerCount(self, p1, p2):
        d1 = abs(p1.first - p2.first) + 1
        d2 = abs(p1.second - p2.second) + 1
        return max(d1, d2)

    def makeUnit(self, pos1):
        pos = pos1.copy()
        if (pos.first != 0):
            pos.first = pos.first / abs(pos.first)

        if (pos.second != 0):
            pos.second = pos.second / abs(pos.second)

        return pos

    def getValidRowMoves(self,rows,player):
        for i in range(0,12):
            for j in range(0,12):
                if (self.validPosition(Pair(i-self.n,j-self.n))):
                    x = self.checkMarkersLocal(Pair(i-self.n,j-self.n),Pair(0,1),player)
                    if (x.first):
                        rows.append(Move(MoveType['removeRow'],x.second.first.copy(),x.second.second.copy()))
                    x = self.checkMarkersLocal(Pair(i-self.n,j-self.n),Pair(1,1),player)
                    if (x.first):
                        rows.append(Move(MoveType['removeRow'],x.second.first.copy(),x.second.second.copy()))
                    x = self.checkMarkersLocal(Pair(i-self.n,j-self.n),Pair(1,0),player)
                    if (x.first):
                        rows.append(Move(MoveType['removeRow'],x.second.first.copy(),x.second.second.copy()))
        return 
        # rows = self.checkMarkers(prevMoveRing.finalPosition.copy(), prevMoveRing.initPosition.copy(), player)

        # for i in range(0,len(rows)):
        #     row = rows[i]
        #     start = row.first.copy()
        #     end = row.second.copy()
        #     if ( self.inclusiveMarkerCount(row.first, row.second) > 5):
        #         direction = Pair(1,2)
        #         direction.first = end.first - start.first
        #         direction.second = end.second - start.second
        #         direction =  self.makeUnit(direction)

        #         alternateEnd = Pair(1,2)
        #         alternateEnd.first = start.first + 4 * direction.first
        #         alternateEnd.second = start.second + 4 * direction.second

        #         moves.append(Move(MoveType['removeRow'], start.copy(), alternateEnd.copy()))

        #         alternateStart = Pair(1,2)
        #         alternateStart.first = end.first - 4 * direction.first
        #         alternateStart.second = end.second - 4 * direction.second

        #         moves.append(Move(MoveType['removeRow'], alternateStart.copy(), end.copy()))
        #     else:
        #         moves.append(Move(MoveType['removeRow'], start.copy(), end.copy()))

floyd_player = floydPlayer()
