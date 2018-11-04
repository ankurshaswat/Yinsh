SRC=  src/Move.cpp src/EvaluatedMoveSeq.cpp src/Util.cpp src/Board.cpp src/AI.cpp src/Interface.cpp src/Simulator.cpp src/main.cpp 
COMMON_FLAGS = -std=c++11 -O3 -o yinsh $(SRC)

all: $(SRC)
	  g++ $(COMMON_FLAGS)

debug: $(SRC)
	  g++ -g $(COMMON_FLAGS)

debug_out: $(SRC)
	  g++ -DUSEDEBUG -g $(COMMON_FLAGS)

warnings: $(SRC)
	  g++ -g -Wall -Wextra $(COMMON_FLAGS)

clean:
	rm -rf build main yinsh