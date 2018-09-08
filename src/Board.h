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

enum class moveType{
    placeRing=0,
    moveRing=1,
    removeRow=2
};
struct Move{
  moveType m;
  pair<int,int> p1,p2;
  Move(): m(moveType::placeRing), p1(make_pair(-9,-9)), p2(make_pair(-9,-9)){}
  bool operator<(const Move& rhs){
    return p1<rhs.p1;
  } 
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

  int getPosition(pair<int, int> position);
  int getPosition(int pos1, int pos2);
  bool validPosition(pair<int, int> position);
  bool validPlaceRing(pair<int, int> position);
  bool validMoveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player);
  void setState(pair<int, int> position, int positionVal);
  void setState(int position1, int position2, int positionVal);
  void invertState(int pos1, int pos2);
  void removeMarker(pair<int, int> position);
  pair<bool, pair<pair<int, int>, pair<int, int>>> checkMarkersLocal(pair<int, int> position, pair<int, int> direction, bool player);
  vector<pair<pair<int, int>, pair<int, int>>> checkMarkersLine(pair<int, int> oldPosition, pair<int, int> newPosition, bool player);

public:
  Board();
  Board(int n);

  void removeMarkers(pair<int, int> startPosition, pair<int, int> endPosition);
  void removeRing(pair<int, int> position);
  bool placeRing(pair<int, int> position, bool player);
  bool moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player);
  vector<pair<pair<int, int>, pair<int, int>>> checkMarkers(pair<int, int> newPosition, pair<int, int> oldPosition, bool player);

  void playMove(Move m);
  void undoMove(Move m);

  /* Return valid moves for player (append to passed vector) */
  void getValidMoves(vector<Move>& moves, bool player);

  /* Return evaluation of board for player */
  int evaluate(bool player);

  /* Check if it is a win for player */
  bool is_win(bool player);

};



#endif /* UTIL_H */
