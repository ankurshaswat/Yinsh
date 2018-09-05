#include "Board.h"

enum positionStates
{
    empty = 0,
    whiteRing = 1,
    blackRing = -1,
    whiteMarker = 2,
    blackMarker = -2
};

Board::Board() : Board(5) {}

Board::Board(int n)
{
    this->n = n;
    board = new int *[2 * n + 1];
    for (int i = 0; i < 2 * n + 2; i++)
    {
        board[i] = new int[2 * n + 1];
        for (int j = 0; j < 2 * n + 2; j++)
        {
            board[i][j] = positionStates::empty;
        }
    }
}

int Board::getPosition(pair<int, int> position)
{
    return board[position.first + n][position.second + n];
}

int Board::getPosition(int pos1, int pos2)
{
    return board[pos1 + n][pos2 + n];
}

bool Board::validPosition(pair<int, int> position)
{
    if (position.first == 0 || position.second == 0 || position.first == position.second)
    {
        return (position.first < n && position.second < n && position.first > -1 * n && position.second > -1 * n);
    }

    return (position.first <= n && position.second <= n && position.first >= -1 * n && position.second >= -1 * n);
}

bool Board::validPlaceRing(pair<int, int> position)
{
    return (validPosition(position) && getPosition(position) == positionStates::empty);
}

bool Board::validMoveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player)
{
    if (newPosition == currentPosition)
    {
        return false;
    }

    if (!(validPosition(newPosition)))
    {
        return false;
    }

    if (!(newPosition.first == currentPosition.first || newPosition.second == currentPosition.second || newPosition.first - currentPosition.first == newPosition.second - currentPosition.second))
    {
        return false;
    }

    int opposingRingState = player ? positionStates::blackRing : positionStates::whiteRing;

    if (newPosition.first == currentPosition.first)
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i = currentPosition.second + increment; i != newPosition.second; i += increment)
        {
            if (getPosition(currentPosition.first, i) == opposingRingState)
            {
                return false;
            }
        }
    }
    else if (newPosition.second == currentPosition.second)
    {
        int increment = newPosition.first < currentPosition.first ? -1 : 1;

        for (int i = currentPosition.first + increment; i != newPosition.first; i += increment)
        {
            if (getPosition(i, currentPosition.second) == opposingRingState)
            {
                return false;
            }
        }
    }
    else
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;
        for (int i1 = currentPosition.first + increment, i2 = currentPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            if (getPosition(i1, i2) == opposingRingState)
            {
                return false;
            }
        }
    }

    return true;
}

void Board::setState(pair<int, int> position, int positionVal)
{
    board[position.first + n][position.second + n] = positionVal;
}

void Board::invertState(int pos1, int pos2)
{
    board[pos1 + n][pos2 + n] *= -1;
}

bool Board::placeRing(pair<int, int> position, bool player)
{

    int playerRing = player ? positionStates::whiteRing : positionStates::blackRing;

    if (validPlaceRing(position))
    {
        setState(position, playerRing);
        return true;
    }

    return false;
}

bool Board::moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player)
{
    if (!validMoveRing(newPosition, currentPosition, player))
    {
        return false;
    }

    int playerRing = player ? positionStates::whiteRing : positionStates::blackRing;
    int playerMarker = player ? positionStates::whiteMarker: positionStates::blackMarker;
    setState(currentPosition, playerMarker);
    setState(newPosition, playerRing);

    if (newPosition.first == currentPosition.first)
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i = currentPosition.second + increment; i != newPosition.second; i += increment)
        {
            invertState(currentPosition.first, i);
        }
    }
    else if (newPosition.second == currentPosition.second)
    {
        int increment = newPosition.first < currentPosition.first ? -1 : 1;

        for (int i = currentPosition.first + increment; i != newPosition.first; i += increment)
        {
            invertState(i, currentPosition.second);
        }
    }
    else
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i1 = currentPosition.first + increment, i2 = currentPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            invertState(i1, i2);
        }
    }

    return true;
}