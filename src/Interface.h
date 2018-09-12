#ifndef INTERFACE_H
#define INTERFACE_H

#include "AI.h"

using namespace std;

class Inteface
{
private:
  Board *board;
  AI *ai;
  bool currentPlayer = 0;

public:
  Inteface();
  Move readInput();
};

#endif