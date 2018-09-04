 all: src/Board.cpp src/Yinsh.cpp
	  g++ -o yinsh src/Board.cpp src/Yinsh.cpp

debug: src/Board.cpp src/Yinsh.cpp
	  g++ -g -o yinsh src/Board.cpp src/Yinsh.cpp
