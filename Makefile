SRC=src/Board.cpp src/Yinsh.cpp src/main.cpp

all: $(SRC)
	  g++ -o yinsh $(SRC)

debug: $(SRC)
	  g++ -g -o yinsh $(SRC)

clean:
	rm -rf build
	rm -rf main
	rm -rf yinsh