/* 
 * File:   Yinsh.h
 * Author: Shashwat Shivam
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include <utility>

using namespace std;

class Board
{
  private:
    int **board;
    int n;

    int getPosition(pair<int, int> position);
    int getPosition(int pos1,int pos2);
    bool validPosition(pair<int, int> position);
    bool validPlaceRing(pair<int, int> position);
    bool validMoveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player);
    void setState(pair<int, int> position, int positionVal);
    void invertState(int pos1,int pos2);

  public:
    Board();
    Board(int n);

    bool placeRing(pair<int, int> position, bool player);
    bool moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player);
};

#endif /* UTIL_H */
