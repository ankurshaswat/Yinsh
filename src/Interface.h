#ifndef INTERFACE_H
#define INTERFACE_H

#include "AI.h"

class Interface
{
private:
  Board *board;
  AI *ai;
  bool currentPlayer = 0;
  Move readInput();

public:
  Interface(bool initBoard,string boardPath);
};

#endif