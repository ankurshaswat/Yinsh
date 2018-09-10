#include "Move.h"

Move::Move() : type(MoveType::placeRing), initPosition(make_pair(-9, -9)), finalPosition(make_pair(-9, -9)) {}
Move::Move(const Move &other) : type(other.type), initPosition(other.initPosition), finalPosition(other.finalPosition) {}
Move &Move::operator=(const Move &other)
{
    if (this != &other)
    {
        type = other.type;
        initPosition = other.initPosition;
        finalPosition = other.finalPosition;
    }
    return *this;
}
Move::Move(MoveType type, pair<int, int> initPosition, pair<int, int> finalPosition) : type(type), initPosition(initPosition), finalPosition(finalPosition) {}