#ifndef INTERFACE_H
#define INTERFACE_H

#include "AI.h"

class Inteface
{
private:
  Board *board;
  AI *ai;
  bool currentPlayer = 0;
  Move readInput();

public:
  Inteface();
};

#endif