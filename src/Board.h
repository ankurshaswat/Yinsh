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

  int getPosition(pair<int, int> position);
  int getPosition(int pos1, int pos2);
  bool validPosition(pair<int, int> position);
  bool validPlaceRing(pair<int, int> position);
  bool validMoveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player);
  void setState(pair<int, int> position, int positionVal);
  void invertState(int pos1, int pos2);
  void removeMarker(pair<int, int> position);
  void removeRing(pair<int, int> position);
  pair<bool, pair<pair<int, int>, pair<int, int>>> checkMarkersLocal(pair<int, int> position, pair<int, int> direction, bool player);
  vector<pair<pair<int, int>, pair<int, int>>> checkMarkersLine(pair<int, int> oldPosition, pair<int, int> newPosition, bool player);

public:
  Board();
  Board(int n);

  bool placeRing(pair<int, int> position, bool player);
  bool moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player);
  vector<pair<pair<int, int>, pair<int, int>>> checkMarkers(pair<int, int> newPosition, pair<int, int> oldPosition, bool player);
};

#endif /* UTIL_H */
