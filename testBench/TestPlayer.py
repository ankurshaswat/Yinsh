from game import Game
import random
import sys
import time

class TestPlayer:

	def __init__(self):
		data = sys.stdin.readline().strip().split() # Initialize Environment
		self.player = 0 #int(data[0]) - 1 # player can have values 0 and 1
		self.n = 5 #int(data[1]) # n can have values 5, 6, or 7
		self.time_left = 150 # int(data[2])
		self.seq=5# int(data[3])
		self.game = Game(self.n,self.seq)
		self.RingPos = {}
		self.play()

	def placeRing(self):
		movetype = 'P'
		hexagon = random.randint(0,self.n)
		position = random.randint(0,max(0,6*hexagon-1))
		if hexagon==self.n and position%self.n==0:
			position+=1
		return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position), len(self.RingPos), hexagon, position

	def selectRing(self):
		movetype = 'S'
		ring_num = random.randint(0,self.n-1)
		while ring_num not in self.RingPos:
			ring_num = random.randint(0,self.n-1)
		ring = self.RingPos[ring_num]
		return '{type} {hex} {pos}'.format(type=movetype, hex=ring[0], pos=ring[1]), ring_num

	def moveRing(self):
		movetype = 'M'
		hexagon = random.randint(0,self.n)
		position = random.randint(0,max(0,6*hexagon-1))
		if hexagon==self.n and position%self.n==0:
			position+=1
		return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position), hexagon, position

	def removeRowStart(self):
		movetype = 'RS'
		hexagon = random.randint(0,self.n)
		position = random.randint(0,max(0,6*hexagon-1))
		if hexagon==self.n and position%self.n==0:
			position+=1
		return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position)

	def removeRowEnd(self):
		movetype = 'RE'
		hexagon = random.randint(0,self.n)
		position = random.randint(0,max(0,6*hexagon-1))
		if hexagon==self.n and position%self.n==0:
			position+=1
		return '{type} {hex} {pos}'.format(type=movetype, hex=hexagon, pos=position)

	def removeRing(self):
		movetype = 'X'
		ring_num = random.randint(0,self.n-1)
		while ring_num not in self.RingPos:
			ring_num = random.randint(0,self.n-1)
		ring = self.RingPos[ring_num]
		return '{type} {hex} {pos}'.format(type=movetype, hex=ring[0], pos=ring[1]), ring_num

	def play_move_seq(self, move_seq):
		moves = ' '.join(move_seq) + '\n'
		sys.stdout.write(moves)
		sys.stdout.flush()

	def play(self):
		if self.player == 1:
			move = sys.stdin.readline().strip()
			# self.game.execute_move(move)

		counter=0

		moveList1=['P 1 3',
		'P 3 4',
		'P 2 3',
		'P 2 1',
		'P 3 11',
		'S 3 11 M 3 10',
		'S 3 10 M 2 6',
		'S 2 6 M 4 14',
		'S 1 3 M 2 7',
		'S 2 3 M 1 0',
		'S 2 7 M 4 10',
		'S 4 10 M 3 7',
		'S 3 4 M 5 12',
		'S 4 14 M 3 10',
		'S 2 1 M 4 11',
		'S 3 7 M 4 13',
		'S 5 12 M 3 3',
		'S 3 10 M 2 7',
		'S 2 7 M 3 13 RS 2 7 RE 5 12 X 4 11',
		'S 3 13 M 3 12',
		'S 4 13 M 5 17',
		'S 1 0 M 1 3',
		'S 5 17 M 5 16',
		'S 3 3 M 3 1',
		'S 3 1 M 2 10',
		'S 3 12 M 4 16',
		'S 5 16 M 4 10 RS 3 12 RE 4 11 X 4 10',
		'S 4 16 M 4 15',
		'S 1 3 M 3 14']

		while True: # Keep playing moves till game is over
			print('#'+str(counter) )
			if(counter==len(moveList1)):
				print("#Moves exhausted")
				time.sleep(1000)
			sys.stdout.write(moveList1[counter]+'\n')
			sys.stdout.flush()
			counter+=1


			move = sys.stdin.readline().strip()

test_player = TestPlayer()
