/* 
 * File:   Util.cpp
 * Author: Shashwat Shivam
 *
 */

#include "Util.h"

#ifdef USEDEBUG
#define Debug(x) std::cout << "# " << x
#else
#define Debug(x)
#endif

pair<int, int> hex2axial(pair<int, int> hexCoord)
{
    Debug("# Util::hex2axial - Input=(" << hexCoord.first << ',' << hexCoord.second << ')' << endl);

    int c1 = hexCoord.first;
    if (c1 == 0)
        return make_pair(0, 0);
    int c2 = hexCoord.second % hexCoord.first;
    int branch = hexCoord.second / hexCoord.first;

    switch (branch)
    {
    case 0:
        return make_pair(c2, c1);
    case 1:
        return make_pair(c1, c1 - c2);
    case 2:
        return make_pair(c1 - c2, -c2);
    case 3:
        return make_pair(-c2, -c1);
    case 4:
        return make_pair(-c1, c2 - c1);
    case 5:
        return make_pair(c2 - c1, c2);
    default:
        Debug("# Util::hex2axial Unknown Case" << endl);
        return make_pair(0, 0);
    }
}

pair<int, int> axial2hex(pair<int, int> axialCoord)
{
    int a = axialCoord.first;
    int b = axialCoord.second;

    int hex, pos;

    if (a == 0 && b == 0)
    {
        hex = 0;
        pos = 0;
    }
    else if (a <= 0 && b >= 0)
    {
        hex = b - a;
        pos = (5 * hex + b) % (6 * hex);
    }
    else if (a >= 0 && b <= 0)
    {
        hex = a - b;
        pos = 3 * hex - a;
    }
    else if (a >= 0 && b >= 0)
    {
        if (b > a)
        {
            hex = b;
            pos = a;
        }
        else
        {
            hex = a;
            pos = 2 * hex - b;
        }
    }
    else
    {
        if (b > a)
        {
            hex = -a;
            pos = 5 * hex + b;
        }
        else
        {
            hex = -b;
            pos = 3 * hex - a;
        }
    }

    return make_pair(hex, pos);
}

pair<int, pair<int, int>> extractCoordinates(string str, int startPosition)
{

    int i = startPosition;

    int coord1 = 0;
    while (str[i] != ' ')
        coord1 = 10 * coord1 + str[i++] - '0';
    i++;
    int coord2 = 0;
    while (str[i] != ' ' && i < str.length())
        coord2 = 10 * coord2 + str[i++] - '0';

    return make_pair(i, hex2axial(make_pair(coord1, coord2)));
}

int inclusiveMarkerCount(pair<int, int> p1, pair<int, int> p2)
{
    int d1 = abs(p1.first - p2.first) + 1;
    int d2 = abs(p1.second - p2.second) + 1;
    return max(d1, d2);
}

pair<int, int> makeUnit(pair<int, int> pos)
{
    if (pos.first != 0)
        pos.first = pos.first / abs(pos.first);

    if (pos.second != 0)
        pos.second = pos.second / abs(pos.second);

    return pos;
}

void printPointers(vector<vector<Move>> moveSequences)
{
    for (int i = 0; i < moveSequences.size(); i++)
    {
        Debug("# ");
        for (int j = 0; j < moveSequences[i].size(); j++)
            Debug(&moveSequences[i][j] << ' ');
        Debug(endl);
    }
}

bool isIntersecting(Move m, Move n)
{

    pair<int, int> pos1 = m.initPosition;
    pair<int, int> pos2 = m.finalPosition;

    int minV, maxV;
    int coordDiff = pos1.first - pos1.second;
    if (pos1.first == pos2.first)
    {
        minV = min(pos1.second, pos2.second);
        maxV = max(pos1.second, pos2.second);
    }
    else
    {
        minV = min(pos1.first, pos2.first);
        maxV = max(pos1.first, pos2.first);
    }

    pair<int, int> startIter = n.initPosition;
    pair<int, int> endIter = n.finalPosition;
    pair<int, int> direction = makeUnit(make_pair(endIter.first - startIter.first, endIter.second - startIter.second));

    pair<int, int> checkPosition = startIter;

    while (!(checkPosition.first == endIter.first + direction.first && checkPosition.second == endIter.second + direction.second))
    {
        if (pos1.first == pos2.first)
        {
            if (checkPosition.first == pos1.first && (checkPosition.second >= minV && checkPosition.second <= maxV))
                return true;
        }
        else if (pos1.second == pos2.second)
        {
            if (checkPosition.second == pos1.second && (checkPosition.first >= minV && checkPosition.first <= maxV))
                return true;
        }
        else
        {
            if (checkPosition.first - checkPosition.second == coordDiff && (checkPosition.first >= minV && checkPosition.first <= maxV))
                return true;
        }

        checkPosition.first += direction.first;
        checkPosition.second += direction.second;
    }

    return false;
}