SRC=  src/Move.cpp src/EvaluatedMoveSeq.cpp src/Util.cpp src/Board.cpp src/AI.cpp src/Interface.cpp src/main.cpp 

all: $(SRC)
	  g++ -o yinsh $(SRC)

debug: $(SRC)
	  g++ -g -o yinsh $(SRC)

debug_out: $(SRC)
	  g++ -DUSEDEBUG -g -o yinsh $(SRC)

warnings: $(SRC)
	  g++ -g -Wall -Wextra -o yinsh $(SRC)

clean:
	rm -rf build
	rm -rf main
	rm -rf yinsh
