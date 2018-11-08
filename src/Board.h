/* 
 * File:   Board.h
 * Author: Shashwat Shivam
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Move.h"

using namespace std;

class Board
{
private:
  enum PositionStates
  {
    empty = 0,
    whiteRing = 1,
    blackRing = -1,
    whiteMarker = 2,
    blackMarker = -2
  };

  enum FeatureIndexes
  {
    TwoMarker,
    ThreeMarker,
    FourMarker,
    FiveMarker,
    SixOrMoreMarker,
    TwoMarkerRing,
    ThreeMarkerRing,
    FourMarkerRing,
    FiveMarkerRing,
    SixOrMoreMarkerRing,
    DistRing1,
    DistRing2,
    DistRing3,
    DistRing4,
    DistRing5,
    DistRing6,
    MarkersCount,
    RingsCount,
    Ring1Freedom,
    Ring2Freedom,
    Ring3Freedom,
    Ring4Freedom,
    Ring5Freedom,
    Ring6Freedom,
    Ring1JumpPositions,
    Ring2JumpPositions,
    Ring3JumpPositions,
    Ring4JumpPositions,
    Ring5JumpPositions,
    Ring6JumpPositions,
    FirstRingRemoved,
    SecondRingRemoved,
  };

  const int numFeatures = FeatureIndexes::SecondRingRemoved + 1;

  int **board;
  int board_size;
  int max_rings;
  int consecutiveMarkers;
  int *counts;

  int test = 0; //for testing

  vector<pair<int, int>> directions;
  vector<vector<pair<int, int>>> rings;

  bool validPosition(pair<int, int> position);
  bool validPlaceRing(pair<int, int> position);
  bool validMoveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player);
  void setState(pair<int, int> position, int positionVal);
  void setState(int initPosition, int finalPosition, int positionVal);
  void invertState(int pos1, int pos2);
  void removeMarker(pair<int, int> position);
  pair<bool, pair<pair<int, int>, pair<int, int>>> checkMarkersLocal(pair<int, int> position, pair<int, int> direction, bool player);
  vector<pair<pair<int, int>, pair<int, int>>> checkMarkersLine(pair<int, int> oldPosition, pair<int, int> newPosition, bool player);
  void placeMarkers(pair<int, int> startPosition, pair<int, int> endPosition, bool player);
  bool counter(pair<int, int> &position, bool &validStartFound, int &prevState, int &countSingleType, int &countWithRing, vector<vector<double>> &featureCounts);

  vector<pair<int, int>> getRingPositions(bool player);

  int getState(pair<int, int> position);
  int getState(int pos1, int pos2);

  vector<pair<pair<int, int>, pair<int, int>>> checkMarkers(pair<int, int> newPosition, pair<int, int> oldPosition, bool player);

  /* Private member access functions */
  int getSize();

public:
  Board();
  Board(int n, int k);

  void featureGenerator(vector<vector<double>> &featureCounts);

  int getNumFeatures();

  void playMove(Move m, bool player);
  void undoMove(Move m, bool player);

  /* Check if it is a win for player */
  bool isWin(bool player);

  int getRingsCount(bool player);

  /* Return evaluation of board for player */
  double evaluate(bool player, int moveCount, vector<double> &featureWeights);

  /* Return valid moves for player (append to passed vector) */
  void getValidRingMoves(vector<Move> &moves, bool player);
  void getValidRowMoves(Move prevMoveRing, vector<Move> &moves, bool player);
  void getValidRemoveRingMoves(vector<Move> &moves, bool player);
  void getValidPlaceRingMoves(vector<Move> &moves, bool player);

  bool placeRing(pair<int, int> position, bool player);
  bool moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player); // Does not require player (Modify later)
  void removeMarkers(pair<int, int> startPosition, pair<int, int> endPosition);
  void removeRing(pair<int, int> position);

  ~Board();
};

#endif /* UTIL_H */
