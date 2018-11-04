/* 
 * File:   main.cpp
 * Author: Shashwat Shivam
 *
 */

#include <iostream>
#include "Interface.h"
#include "Simulator.h"

int main(int argc, char const *argv[])
{
    auto t = time(0);
    srand(t);

    string arg = argv[1];
    if (arg == "0")
    {
        Inteface *interface = new Inteface();
    }
    else
    {
        string resultsPath = argv[2];
        int selfPolicy = stoi(argv[3]);
        int oppPolicy = stoi(argv[4]);
        string weightsFilePath = argv[5];
        int numGames = stoi(argv[6]);
        Simulator *simulator = new Simulator(resultsPath, selfPolicy, oppPolicy, weightsFilePath);

        simulator->simulate(numGames);
    }

    return 0;
}
