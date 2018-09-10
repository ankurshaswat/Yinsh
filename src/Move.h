/* 
 * File:   Move.h
 * Author: Shashwat Shivam
 *
 */

#ifndef MOVE_H
#define MOVE_H

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
    Move();
    Move(const Move &other);
    Move &operator=(const Move &other);
    Move(MoveType type, pair<int, int> initPosition, pair<int, int> finalPosition);
};

#endif