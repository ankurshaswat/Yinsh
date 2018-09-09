/* 
 * File:   Yinsh.h
 * Author: Shashwat Shivam
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>

using namespace std;

enum class MoveType
{
  placeRing = 0,
  moveRing = 1,
  removeRow = 2,
  removeRing = 3
};
struct Move
{
  MoveType type;
  pair<int, int> initPosition, finalPosition;
  Move() : type(MoveType::placeRing), initPosition(make_pair(-9, -9)), finalPosition(make_pair(-9, -9)) {}
  Move(MoveType type, pair<int, int> initPosition, pair<int, int> finalPosition) : type(type), initPosition(initPosition), finalPosition(finalPosition) {}
};

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

  int **board;
  int n;
  int *counts;

  vector<pair<int, int>> directions;
  vector<pair<int, int>> rings0;
  vector<pair<int, int>> rings1;

  int getState(pair<int, int> position);
  int getState(int pos1, int pos2);
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

public:
  Board();
  Board(int n);

  void removeMarkers(pair<int, int> startPosition, pair<int, int> endPosition);
  void removeRing(pair<int, int> position);
  bool placeRing(pair<int, int> position, bool player);
  bool moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player); // Does not require player (Modify later)
  vector<pair<pair<int, int>, pair<int, int>>> checkMarkers(pair<int, int> newPosition, pair<int, int> oldPosition, bool player);

  void playMove(Move m, bool player);
  void undoMove(Move m, bool player);

  /* Return valid moves for player (append to passed vector) */
  void getValidRingMoves(vector<Move> &moves, bool player);
  void getValidRowMoves(Move prevMoveRing, vector<Move> &moves, bool player);

  /* Return evaluation of board for player */
  int evaluate(bool player);

  /* Check if it is a win for player */
  bool isWin(bool player);
};

#endif /* UTIL_H */
