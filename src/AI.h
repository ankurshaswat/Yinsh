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
    
    void playMoveSeq();

    /* Returns best move for min node (and its evaluation) */
    pair<Move,int> minValue(int alpha, int beta, int depth, Board& board);
    
    /* Returns best move for max node (and its evaluation) */
    pair<Move,int> maxValue(int alpha, int beta, int depth, Board& board);
    
};

#endif