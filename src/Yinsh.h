/* 
 * File:   Yinsh.h
 * Author: Shashwat Shivam
 *
 */

#ifndef YINSH_H
#define YINSH_H

#include<utility>
#include "Board.h"

using namespace std;

/**
 * Yinsh gives the description of the class to represent the whole board state.
 * 
 * @author Shashwat Shivam
 *
 */
class Yinsh
{
  private:
    int n, m, k, l;
    bool activePlater; // 0 for white
    Board *board;
    pair<int,int> scores;

    enum class TurnModes {
        AddRing,
        AddMarker,
        RemoveRun,
        RemoveRing,
        MoveRing
    };

    TurnModes turnMode;

  public:
    Yinsh();
    Yinsh(int n, int m, int k, int l);
};

#endif /* YINSH_H */