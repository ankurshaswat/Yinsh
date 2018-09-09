#ifndef AI_H
#define AI_H

#include "Board.h"

class AI
{
private:
  Board *board;
  bool player;
  double time;
  int moveCount;
  int boardSize;

public:
  AI(Board *board, bool player, int time);

  void playMoveSeq();

  /* Returns best move for min node (and its evaluation) */
  pair<vector<Move>, int> minValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player);

  /* Returns best move for max node (and its evaluation) */
  pair<vector<Move>, int> maxValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player);

  void AI::RowMoves(Board& board, bool player, vector<Move>& removeRowMoves, vector<Move>& moveSeq, vector< vector<Move> > & moveSequences, bool continuePlaying);
  void AI::moveMarkerMoves(Board& board, vector<Move>& moveSeq, vector< vector<Move> > & moveSequences, bool player);

};

#endif