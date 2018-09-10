/* 
 * File:   Util.h
 * Author: Shashwat Shivam
 *
 */

#include "Util.h"
#include <iostream>
#include <utility>
using namespace std;

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
    {
        return make_pair(0, 0);
    }
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

    Debug("# Util::axial2hex - Input=(" << a << ',' << b << ')' << " Output=(" << hex << ',' << pos << ')' << endl);
    return make_pair(hex, pos);
}

pair<int, pair<int, int>> extractCoordinates(string str, int startPosition)
{

    int i = startPosition;

    int coord1 = 0;
    while (str[i] != ' ')
    {
        coord1 = 10 * coord1 + str[i++] - '0';
    }
    i++;
    int coord2 = 0;
    while (str[i] != ' ' && i < str.length())
    {
        coord2 = 10 * coord2 + str[i++] - '0';
    }

    return make_pair(i, hex2axial(make_pair(coord1, coord2)));
}

int axialDistance(pair<int, int> p1, pair<int, int> p2)
{
    int d1 = abs(p1.first - p2.first);
    int d2 = abs(p1.second - p2.second);
    return max(d1, d2);
}

pair<int, int> makeUnit(pair<int, int> pos)
{
    if (pos.first != 0)
    {
        pos.first = pos.first / abs(pos.first);
    }

    if (pos.second != 0)
    {
        pos.second = pos.second / abs(pos.second);
    }

    return pos;
}

void printPointers(vector<vector<Move>> moveSequences)
{
    for (int i = 0; i < moveSequences.size(); i++)
    {
        Debug("# ");
        for (int j = 0; j < moveSequences[i].size(); j++)
        {
            Debug(&moveSequences[i][j] << ' ');
        }
        Debug(endl);
    }
}
