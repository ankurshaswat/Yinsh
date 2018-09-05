#include "Yinsh.h"
#include "Board.h"

Yinsh::Yinsh() : Yinsh(5, 5, 5, 3) {}

Yinsh::Yinsh(int n, int m, int k, int l)
{
    this->n = n;
    this->m = m;
    this->k = k;
    this->l = l;

    this->turnMode = TurnModes::AddMarker;
    this->scores = make_pair(0, 0);

    board = new Board(n);
}
