#ifndef AI_H
#define AI_H

#include <string>
#include "Board.h"
#include "EvaluatedMoveSeq.h"

class AI
{
private:
  Board *originalBoard;
  bool player;
  double time;
  int moveCount;
  int boardSize;
  int maxRings;
  int consecutiveMarkers;

  vector<double> featureWeights = {1, 5, 25, 75, 150, 2, 10, 50, 75, 150, 1, 1, 1, 1, 1, 1, 1, -2000, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1500, 2500};

  string convertMoveToString(Move m);

  /* Returns best move for min node (and its evaluation) */
  pair<vector<Move>, double> minValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int moveCount);

  /* Returns best move for max node (and its evaluation) */
  pair<vector<Move>, double> maxValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int moveCount);

  void rowMoves(Board &board, bool player, vector<Move> &removeRowMoves, vector<Move> &moveSeq, vector<EvaluatedMoveSeq> &moveSequences, bool continuePlaying, int depth);
  void moveMarkerMoves(Board &board, vector<Move> &moveSeq, vector<EvaluatedMoveSeq> &moveSequences, bool player, int depth);

public:
  AI(Board *board, bool player, int time, int n, int k);

  string playMoveSeq(Move m);
  void setWeights(vector<double> featureWeights);
  void setMoveCount(int moveCount);
};

#endif