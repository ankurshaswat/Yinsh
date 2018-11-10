import sys

path = sys.argv[1]

p1Name = sys.argv[2]
p2Name = sys.argv[3]

p1Wins_S = [0, 0, 0]
p1W_S = 0
p2Wins_S = [0, 0, 0]
p2W_S = 0

totalGames = 0

with open(path+'/gameRun_straight.log') as file:
    gameRes = file.readlines()
    for line in gameRes:
        totalGames += 1
        gameResArr = line.rstrip().split(' ')
        winner = int(gameResArr[1])
        winScore = float(gameResArr[4])
        loser = int(gameResArr[7])
        loserScore = float(gameResArr[10])

        if(winner == 1):
            p1W_S += 1
            if(winScore >= 10.0):
                p1Wins_S[0] += 1
            elif (winScore >= 9.0):
                p1Wins_S[1] += 1
            else:
                p1Wins_S[2] += 1
        else:
            p2W_S += 1
            if(winScore >= 10.0):
                p2Wins_S[0] += 1
            elif (winScore >= 9.0):
                p2Wins_S[1] += 1
            else:
                p2Wins_S[2] += 1

p1Wins_I = [0, 0, 0]
p1W_I = 0
p2Wins_I = [0, 0, 0]
p2W_I = 0

with open(path+'/gameRun_inverted.log') as file:
    gameRes = file.readlines()
    for line in gameRes:
        totalGames += 1
        gameResArr = line.rstrip().split(' ')
        winner = int(gameResArr[1])
        winScore = float(gameResArr[4])
        loser = int(gameResArr[7])
        loserScore = float(gameResArr[10])

        if(winner == 2):
            p1W_I += 1
            if(winScore >= 10.0):
                p1Wins_I[0] += 1
            elif (winScore >= 9.0):
                p1Wins_I[1] += 1
            else:
                p1Wins_I[2] += 1
        else:
            p2W_I += 1
            if(winScore >= 10.0):
                p2Wins_I[0] += 1
            elif (winScore >= 9.0):
                p2Wins_I[1] += 1
            else:
                p2Wins_I[2] += 1

f = open(path + "/stats.log", "w")

f.write("S1 "+p1Name+" wins " + str(100.0*(p1W_S + p1W_I)/totalGames)+"% " + "\n")
f.write("S1 "+p1Name+" wins " + str(100.0*(p1W_S)/(totalGames/2)) +
        "% of games played as player1" + "\n")
f.write("S1 "+p1Name+" wins " + str(100.0*(p1W_I)/(totalGames/2)) +
        "% of games played as player2" + "\n")
f.write("Win numbers\n")
f.write("Score AsP1 AsP2\n")
f.write("3-0 "+str(p1Wins_S[0])+" " + str(p1Wins_I[0])+"\n")
f.write("3-1 "+str(p1Wins_S[1])+" " + str(p1Wins_I[1])+"\n")
f.write("3-2 "+str(p1Wins_S[2])+" " + str(p1Wins_I[2])+"\n")

f.write("S2 "+p2Name+" wins "+str(100.0*(p2W_S+p2W_I)/totalGames)+"% " + "\n")
f.write("S2 "+p2Name+" wins " + str(100.0*(p2W_S)/(totalGames/2)) +
        "% of games played as player1" + "\n")
f.write("S2 "+p2Name+" wins " + str(100.0*(p2W_I)/(totalGames/2)) +
        "% of games played as player2" + "\n")
f.write("Win numbers\n")
f.write("Score AsP1 AsP2\n")
f.write("3-0 "+str(p2Wins_S[0])+" " + str(p2Wins_I[0])+"\n")
f.write("3-1 "+str(p2Wins_S[1])+" " + str(p2Wins_I[1])+"\n")
f.write("3-2 "+str(p2Wins_S[2])+" " + str(p2Wins_I[2])+"\n")

f.close()
