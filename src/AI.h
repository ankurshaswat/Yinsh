#ifndef AI_H
#define AI_H

#include "Board.h"

class AI
{
  private:
    Board *board;
    bool player;
    double time;
    
  public:
    AI(Board *board, bool player,int time);
    void getMove();
};

#endif