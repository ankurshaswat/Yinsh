#ifndef AI_H
#define AI_H

#include "Board.h"

class AI
{
private:
  Board *originalBoard;
  bool player;
  double time;
  int moveCount;
  int boardSize;
  int n;

public:
  AI(Board *board, bool player, int time, int n);

  void playMoveSeq(Move m);

  /* Returns best move for min node (and its evaluation) */
  pair<vector<Move>, int> minValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int moveCount);

  /* Returns best move for max node (and its evaluation) */
  pair<vector<Move>, int> maxValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int moveCount);

  void rowMoves(Board &board, bool player, vector<Move> &removeRowMoves, vector<Move> &moveSeq, vector<vector<Move>> &moveSequences, bool continuePlaying);
  void moveMarkerMoves(Board &board, vector<Move> &moveSeq, vector<vector<Move>> &moveSequences, bool player);
};

#endif